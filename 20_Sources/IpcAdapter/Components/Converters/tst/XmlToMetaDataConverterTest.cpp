#include "XmlToMetaDataConverterTest.h"
#include "Components/Converters/src/XmlToMetaDataConverter.h"
#include "Core/api/IConfigurable.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/SimplePipelineFrame.h"
#include "Shared/tst/QTestConvenienceMacros.h"



using IpcAdapter::Components::Converters::XmlToMetaDataConverterTest;
using IpcAdapter::Components::Converters::XmlToMetaDataConverter;
using IpcAdapter::Core::SimplePipelineFrame;



namespace
{
    void testConfiguration(
        std::function<void(IpcAdapter::Core::IConfigurable&)> configurationCallback,
        bool expectConfigurationComplete,
        char const* const configurationCompleteErrorMessage
    )
    {
        auto uut = std::make_unique<XmlToMetaDataConverter>();

        TEST_REQUIREMENT("R-IPCA-XML2METACNV-002");
        auto& configurable = *uut->get();
        {
            configurable.onConfigureBegin();
            configurationCallback(configurable);
            COMPARE(configurable.onConfigureEnd(), expectConfigurationComplete, configurationCompleteErrorMessage);
        }
    }
}



XmlToMetaDataConverterTest::XmlToMetaDataConverterTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("R-IPCA-XML2METACNV-001");
}



void XmlToMetaDataConverterTest::test_00_XmlToMetaDataConverter_is_published()
{
    VERIFY(
        IpcAdapter::Core::GlobalComponentRegistry::get().getFactoryFor("XmlToMetaDataConverter") != nullptr,
        "Ensure the XmlToMetaDataConverter is published to global registry"
    );
}



void XmlToMetaDataConverterTest::test_01_configuration_fails_for_unknown_parameter()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("unknown", "value"), false, "using an unknown parameter must fail");
    }, true, "configuration shall succeed");
}



void XmlToMetaDataConverterTest::test_02_configuration_fails_for_non_boolean_filter()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("filter-on-broken-xml", "x"), false, "using non-boolean must fail");
    }, true, "configuration shall succeed");
}


void XmlToMetaDataConverterTest::test_03_configuration_fails_for_non_boolean_forward()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("forward-raw-data", "x"), false, "using non-boolean must fail");
    }, true, "configuration shall succeed");
}


void XmlToMetaDataConverterTest::test_10_valid_processing()
{
    auto uut = std::make_unique<XmlToMetaDataConverter>();

    auto abortProcessing = false;

    auto const frame = std::make_shared<SimplePipelineFrame>("<root><element id=\"1\" other=\"attribute\"><another info=\"here\"/></element></root>");
    auto const converted = uut->convert(frame, abortProcessing);

    COMPARE(abortProcessing, false, "valid parsing did not abort");

    QMap<QString, QString> const seenParameters
    {
        {"root.element.id", "1"},
        {"root.element.other", "attribute"},
        {"root.element.another.info", "here"}
    };

    auto const& metaData = converted->getMetaData();

    for (auto const& key : seenParameters.keys())
    {
        VERIFY(metaData.contains(key), qPrintable(QString("must contain key: %1").arg(key)));
        COMPARE(metaData.value(key).toString(), seenParameters.value(key), qPrintable(QString("key %1 must match: %2").arg(key).arg(seenParameters.value(key))));
    }

    VERIFY(converted->getData().isEmpty(), "nothing shall be forwarded as XML parsing worked");
}



void XmlToMetaDataConverterTest::test_11_filter_on_broken_xml()
{
    auto uut = std::make_unique<XmlToMetaDataConverter>();

    auto abortProcessing = false;

    auto const frame = std::make_shared<SimplePipelineFrame>("<broken");
    auto const converted = uut->convert(frame, abortProcessing);

    COMPARE(abortProcessing, true, "must have been aborted");
}



void XmlToMetaDataConverterTest::test_12_overwrite_default_parameters_must_work()
{
    auto uut = std::make_unique<XmlToMetaDataConverter>();
    {
        auto configurable = uut->get();
        configurable->onConfigureBegin();
        configurable->doConfigure("filter-on-broken-xml", "false");
        configurable->doConfigure("forward-raw-data", "true");
        configurable->onConfigureEnd();
    }

    auto abortProcessing = false;
    auto const frame = std::make_shared<SimplePipelineFrame>("<broken ");
    auto const converted = uut->convert(frame, abortProcessing);

    COMPARE(abortProcessing, false, "must not have been aborted");
    COMPARE(converted->getData(), QByteArray("<broken "), "data shall have been forwarded");
}
