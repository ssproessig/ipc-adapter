#include "ReverseBytesConverterTest.h"

#include "Components/Converters/src/ReverseBytesConverter.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/SimplePipelineFrame.h"
#include "Shared/tst/QTestConvenienceMacros.h"



using IpcAdapter::Components::Converters::ReverseBytesConverterTest;
using IpcAdapter::Components::Converters::ReverseBytesConverter;



ReverseBytesConverterTest::ReverseBytesConverterTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("R-IPCA-CONVERTER-001");
}



void ReverseBytesConverterTest::test_00_ReverseBytesConverter_is_published()
{
    VERIFY(
        IpcAdapter::Core::GlobalComponentRegistry::get().getFactoryFor("ReverseBytesConverter") != nullptr,
        "Ensure the ReverseBytesConverter is published to global registry"
    );
}


void ReverseBytesConverterTest::test_01_ReverseBytesConverter_is_not_configurable()
{
    ReverseBytesConverter uut;

    COMPARE(uut.getConfigurable(), nullptr, "ReverseBytesConverter shall not return a configurable");
}



void ReverseBytesConverterTest::test_02_ReverseBytesConverter_conversion()
{
    ReverseBytesConverter uut;

    auto shallAbort = false;

    auto const inputFrame = std::make_shared<Core::SimplePipelineFrame>("ABC123");
    auto const outputFrame = uut.convert(inputFrame, shallAbort);
    COMPARE(outputFrame->getData(), QByteArray("321CBA"), "expect the input to be reversed");
    COMPARE(shallAbort, false, "reversing never aborts the pipeline");
}
