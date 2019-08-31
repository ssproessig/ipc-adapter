#include "UdpSinkTest.h"

#include "Components/Sinks/src/UdpSink.h"
#include "Core/api/IConfigurable.h"
#include "Shared/tst/QTestConvenienceMacros.h"

#include <functional>



using IpcAdapter::Components::Sinks::UdpSinkTest;
using IpcAdapter::Components::Sinks::UdpSink;



namespace
{
    void testConfiguration(
        std::function<void(IpcAdapter::Core::IConfigurable&)> configurationCallback,
        bool expectConfigurationComplete,
        char const* const configurationCompleteErrorMessage
    )
    {
        auto uut = std::make_unique<UdpSink>();

        TEST_REQUIREMENT("R-IPCA-SINK-002");
        auto& configurable = uut->getConfigurable();
        {
            configurable.onConfigureBegin();
            configurationCallback(configurable);
            COMPARE(configurable.onConfigureEnd(), expectConfigurationComplete, configurationCompleteErrorMessage);
        }
    }
}



UdpSinkTest::UdpSinkTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("R-IPCA-SINK-001");
}



void UdpSinkTest::test_01_configuration_fails_for_unknown_parameter()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("unknown", "value"), false, "using an unknown parameter must fail");
    }, false, "w/o any configured parameter we failed");
}



void UdpSinkTest::test_02_configuration_succeeds_for_known_parameters_w_correct_value()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("port", "12345"), true, "configuration succeeds for valid 'port' value");
        COMPARE(configurable.doConfigure("host", "127.0.0.1"), true, "configuration succeeds for valid 'host' value");
    }, true, "succeed configuration when all mandatory parameters were configured");
}



void UdpSinkTest::test_03_configuration_fails_if_mandatory_parameter_is_missing()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable&) {}, false, "w/o any configured parameter we failed");
}



void UdpSinkTest::test_04_configuration_fails_for_invalid_port()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("port", "affe"), false, "configuration fails for invalid 'port' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void UdpSinkTest::test_05_configuration_fails_for_invalid_host()
{
    testConfiguration([](IpcAdapter::Core::IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("host", ""), false, "configuration fails for invalid 'host' value");
    }, false, "configuration must fail in case faulty parameter was used");
}
