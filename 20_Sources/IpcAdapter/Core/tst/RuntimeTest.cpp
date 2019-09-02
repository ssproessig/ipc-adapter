#include "RuntimeTest.h"

#include "Core/api/IComponent.h"
#include "Core/api/IConfigurable.h"
#include "Core/api/IRuntimeConfiguration.h"
#include "Core/api/ISink.h"
#include "Core/api/ISource.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/Runtime.h"
#include "Shared/tst/QTestConvenienceMacros.h"


using IpcAdapter::Core::Runtime;
using IpcAdapter::Core::RuntimeTest;
using IpcAdapter::Core::IConfigurable;



namespace
{
    struct TestComponent
        : IpcAdapter::Core::IComponent
        , IpcAdapter::Core::IConfigurable
    {
        IConfigurable* getConfigurable() override
        {
            return isConfigurable ? this : nullptr;
        }
        void onConfigureBegin() override {}
        bool doConfigure(QString const& aKey, QString const& aValue) override
        {
            key = aKey;
            value = aValue;
            return acceptedParameter;
        }
        bool onConfigureEnd() override
        {
            return acceptedConfiguration;
        }

        QString key;
        QString value;

        static bool acceptedParameter;
        static bool acceptedConfiguration;
        static bool isConfigurable;
    };
    REGISTER_COMPONENT_IMPL(TestComponent, TC1, tc1)

    bool TestComponent::acceptedConfiguration = true;
    bool TestComponent::acceptedParameter = true;
    bool TestComponent::isConfigurable = true;

    struct TestSource : IpcAdapter::Core::ISource
    {
        IConfigurable* getConfigurable() override
        {
            return nullptr;
        }

        void sourceTo(IpcAdapter::Core::IPipelineStep* aPipelineStep) override
        {
            throw std::logic_error("The method or operation is not implemented.");
        }
    };
    REGISTER_COMPONENT_IMPL(TestSource, TC2, tc2)

    struct TestSink : IpcAdapter::Core::ISink
    {
        IConfigurable* getConfigurable() override
        {
            return nullptr;
        }

        bool process(IpcAdapter::Core::IPipelineFrame const& aPipelineFrame)
        {
            return false;
        }
    };
    REGISTER_COMPONENT_IMPL(TestSink, TC3, tc3)
}



RuntimeTest::RuntimeTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("R-IPCA-SINK-001");
}



void RuntimeTest::init()
{
    TestComponent::acceptedConfiguration = true;
    TestComponent::acceptedParameter = true;
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
    TestComponent::acceptedConfiguration = false;

    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_08_two_components.xml");,
        "creating Runtime from configuration must fail if component created fails to configure",
        "unable to finish configuring 'id1'!"
    )
}



void RuntimeTest::test_09_Runtime_initialization_fails_if_component_rejects_parameter()
{
    TestComponent::acceptedParameter = false;

    EXPECT_EXCEPTION(
        Runtime::createFrom(":/RuntimeTest_08_two_components.xml");,
        "creating Runtime from configuration must fail if component rejects a parameter",
        "component 'id1' rejects parameter 'aKey' with value 'aValue'!"
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


void RuntimeTest::test_98_Runtime_initialization_succeeds()
{
    auto const uut = Runtime::createFrom(":/RuntimeTest_08_two_components.xml");
    auto const& configuration = uut->getRuntimeConfiguration();
    auto const& components = configuration.getComponents();

    COMPARE(components.count(), 2, "we shall have configured two components");
    COMPARE(components.contains("id1"), true, "we shall have component 'id1'");
    COMPARE(components.contains("id2"), true, "we shall have component 'id2'");

    auto const c1 = std::dynamic_pointer_cast<TestComponent>(components["id1"]);
    QVERIFY(c1 != nullptr);
    COMPARE(c1->key, QString("aKey"), "correct key shall be used");
    COMPARE(c1->value, QString("aValue"), "param shall be set");

    auto const c2 = std::dynamic_pointer_cast<TestComponent>(components["id2"]);
    QVERIFY(c2 != nullptr);
    VERIFY(c2->key.isEmpty(), "2nd component shall not be configured");
    VERIFY(c2->value.isEmpty(), "2nd component shall not be configured");
}



void RuntimeTest::test_99_Runtime_initialization_works_for_component_that_is_not_configurable()
{
    TestComponent::isConfigurable = false;

    auto const uut = Runtime::createFrom(":/RuntimeTest_08_two_components.xml");
    COMPARE(uut->getRuntimeConfiguration().getComponents().count(), 2, "we shall still have configured two components");
}
