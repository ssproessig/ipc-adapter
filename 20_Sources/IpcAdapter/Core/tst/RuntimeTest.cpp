#include "RuntimeTest.h"

#include "Core/api/IComponent.h"
#include "Core/api/IConfigurable.h"
#include "Core/api/IConverter.h"
#include "Core/api/IProvider.h"
#include "Core/api/IRuntimeConfiguration.h"
#include "Core/api/ISink.h"
#include "Core/api/ISource.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/Runtime.h"
#include "Core/api/SimplePipelineFrame.h"
#include "Shared/tst/QTestConvenienceMacros.h"



using IpcAdapter::Core::Runtime;
using IpcAdapter::Core::RuntimeTest;
using IpcAdapter::Core::IConfigurable;
using IpcAdapter::Core::PipelineFramePtr;
using IpcAdapter::Core::SimplePipelineFrame;



namespace
{
    struct Configurable: IpcAdapter::Core::IConfigurable
    {
        void onConfigureBegin() override {}
        bool doConfigure(QString const& aKey, QString const& aValue) override
        {
            paramsSeen.append({ aKey, aValue });
            return acceptedParameter;
        }
        bool onConfigureEnd() override
        {
            return acceptedConfiguration;
        }


        QList<QPair<QString, QString>> paramsSeen;

        static bool acceptedParameter;
        static bool acceptedConfiguration;
    };
    bool Configurable::acceptedConfiguration = true;
    bool Configurable::acceptedParameter = true;

    struct TestComponent
        : IpcAdapter::Core::IComponent
        , IpcAdapter::Core::IProvider<IpcAdapter::Core::IConfigurable>
    {
        IConfigurable* get() const override
        {
            return isConfigurable ? configurablePtr : nullptr;
        }

        Configurable configurable;
        IConfigurable* configurablePtr = &configurable;
        static bool isConfigurable;
    };
    bool TestComponent::isConfigurable = true;

    REGISTER_COMPONENT_IMPL(TestComponent, TC1, tc1)

    struct TestSource : IpcAdapter::Core::ISource
    {
        void sourceTo(IpcAdapter::Core::IPipelineStep* aPipelineStep) override
        {
            forwardTo = aPipelineStep;
        }

        IpcAdapter::Core::IPipelineStep* forwardTo = nullptr;
    };
    REGISTER_COMPONENT_IMPL(TestSource, TC2, tc2)

    struct TestSink : IpcAdapter::Core::ISink
    {
        bool process(IpcAdapter::Core::IPipelineFrame const& aPipelineFrame) override
        {
            framesSeen.append(std::make_shared<SimplePipelineFrame>(aPipelineFrame.getData()));
            return false;
        }

        QList<PipelineFramePtr> framesSeen;
    };
    REGISTER_COMPONENT_IMPL(TestSink, TC3, tc3)

    struct TestConverter : IpcAdapter::Core::IConverter
    {
        PipelineFramePtr convert(PipelineFramePtr const& anInput, bool&) override
        {
            timesInvoked += 1;
            auto data = anInput->getData();
            data.append(QString("+%1").arg(timesInvoked));
            return std::make_shared<SimplePipelineFrame>(data);
        }

        int timesInvoked = 0;
    };
    REGISTER_COMPONENT_IMPL(TestConverter, TC4, tc4)
}



RuntimeTest::RuntimeTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
}



void RuntimeTest::init()
{
    Configurable::acceptedConfiguration = true;
    Configurable::acceptedParameter = true;
    TestComponent::isConfigurable = true;
}



void RuntimeTest::test_01_Runtime_initialization_fails_if_not_existing_file_is_passed()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/unknown.xml");,
        "creating Runtime from ':/unknown.xml' configuration must fail",
        "Given configuration ':/unknown.xml' not found!"
    )
}



void RuntimeTest::test_02_Runtime_initialization_fails_for_broken_file()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_00_broken_file.xml");,
        "creating Runtime from broken file must fail",
        "fatalError [1,1]: error occurred while parsing element"
    )
}



void RuntimeTest::test_03_Runtime_initialization_fails_for_wrong_xml_used()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_03_wrong_xml.xml");,
        "creating Runtime from XML with wrong schema must fail",
        "configuration has unsupported namespace ''!"
    )
}



void RuntimeTest::test_04_Runtime_initialization_fails_for_wrong_root_element()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_04_wrong_root_element.xml");,
        "creating Runtime from XML with correct schema but wrong root element must fail",
        "configuration has wrong root element 'source'!"
    )
}



void RuntimeTest::test_05_Runtime_initialization_fails_for_root_element_with_unsupported_version()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_05_root_element_with_wrong_version.xml");,
        "creating Runtime from XML with correct schema and root element, but unsupported version, must fail",
        "configuration has unsupported version '2'!"
    )
}



void RuntimeTest::test_06_Runtime_initialization_fails_if_unknown_component_shall_be_created()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_06_unknown_component.xml");,
        "creating Runtime from configuration that references unknown component must fail",
        "unknown component 'UnknownComponent' requested!"
    )
}



void RuntimeTest::test_07_Runtime_initialization_fails_if_duplicate_component_shall_be_created()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_07_duplicate_component_id.xml");,
        "creating Runtime from configuration that configures the same id twice MUST fail",
        "unable to create 'TestComponent' again for id 'id1'!"
    )
}



void RuntimeTest::test_08_Runtime_initialization_fails_if_component_fails_to_configure()
{
    Configurable::acceptedConfiguration = false;

    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_08_two_components.xml");,
        "creating Runtime from configuration must fail if component created fails to configure",
        "unable to finish configuring 'id1'!"
    )
}



void RuntimeTest::test_09_Runtime_initialization_fails_if_component_rejects_parameter()
{
    Configurable::acceptedParameter = false;

    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_08_two_components.xml");,
        "creating Runtime from configuration must fail if component rejects a parameter",
        "component 'id1' rejects parameter 'aKey' with value 'aValue'!"
    )
}



void RuntimeTest::test_10_Runtime_initialization_fails_if_unknown_param_list_is_referenced()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_12_unknown_param_list_ref.xml");,
        "creating Runtime from configuration must fail if component references an unknown param-list",
        "component 'id1' references unknown param-list 'shared'!"
    )
}



void RuntimeTest::test_20_Runtime_initialization_fails_if_duplicate_pipeline_id_is_used()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_09_duplicate_pipeline_id.xml");,
        "creating Runtime from configuration must fail if pipeline reuses id",
        "trying to redefine pipeline 'id1'!"
    )
}



void RuntimeTest::test_21_Runtime_initialization_fails_if_pipeline_uses_non_sink_as_sink()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_10_pipeline_with_wrong_component_as_sink.xml");,
        "creating Runtime from configuration must fail if pipeline uses non-sink as <sink/>",
        "component 'source' referenced in 'id1' is no sink!"
    )
}



void RuntimeTest::test_22_Runtime_initialization_fails_if_pipeline_uses_non_converter_as_converter()
{
    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_11_pipeline_with_wrong_component_as_converter.xml");,
        "creating Runtime from configuration must fail if pipeline uses non-converter as <converter/>",
        "component 'source' referenced in 'id1' is no converter!"
    )
}



void RuntimeTest::test_98_Runtime_initialization_succeeds()
{
    auto const uut = Runtime::createFrom(":/RuntimeTest_99_all_features.xml");
    auto const& configuration = uut->getRuntimeConfiguration();
    auto const& components = configuration.getComponents();

    COMPARE(components.count(), 4, "we shall have configured four components");
    COMPARE(components.contains("cmp"), true, "we shall have component 'cmp'");
    COMPARE(components.contains("src"), true, "we shall have component 'src'");
    COMPARE(components.contains("snk"), true, "we shall have component 'snk'");
    COMPARE(components.contains("cnv"), true, "we shall have component 'cnv'");

    auto const c1 = std::dynamic_pointer_cast<TestComponent>(components["cmp"]);
    QVERIFY(c1 != nullptr);
    auto const& c = c1->configurable;
    COMPARE(c.paramsSeen.count(), 3, "expect two parameters to be configured");
    COMPARE(c.paramsSeen.at(0).first, QString("aKey"), "expect 1st parameter from param-list");
    COMPARE(c.paramsSeen.at(0).second, QString("paramListValue1"), "expect 1st parameter from param-list");
    COMPARE(c.paramsSeen.at(1).first, QString("2ndKey"), "expect 2nd parameter from param-list");
    COMPARE(c.paramsSeen.at(1).second, QString("paramListValue2"), "expect 2nd parameter from param-list");
    COMPARE(c.paramsSeen.at(2).first, QString("aKey"), "expect 3rd parameter from local param");
    COMPARE(c.paramsSeen.at(2).second, QString("aValue"), "expect 3rd parameter from local parameter");

    COMPARE(configuration.getPipelines().count(), 2, "we shall have configured two pipelines");

    auto const src = std::dynamic_pointer_cast<TestSource>(components["src"]);
    auto const snk = std::dynamic_pointer_cast<TestSink>(components["snk"]);

    src->forwardTo->process(std::make_shared<SimplePipelineFrame>("Test123"));

    COMPARE(snk->framesSeen.count(), 2, "the sink shall have received two frame: one from id1 and one from i2");
    COMPARE(snk->framesSeen.at(0)->getData(), IPipelineFrame::RawData("Test123"), "id1 only forwards");
    COMPARE(snk->framesSeen.at(1)->getData(), IPipelineFrame::RawData("Test123+1+2"), "id2 forwards the frame through 'cnv' twice");
}



void RuntimeTest::test_99_Runtime_initialization_works_for_component_that_is_not_configurable()
{
    TestComponent::isConfigurable = false;

    auto const uut = Runtime::createFrom(":/RuntimeTest_08_two_components.xml");
    COMPARE(uut->getRuntimeConfiguration().getComponents().count(), 2, "we shall still have configured two components");
}
