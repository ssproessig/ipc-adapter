#include "AmqpConfigurableTest.h"

#include "Components/AmqpComponents/src/AmqpConfigurable.h"
#include "Components/AmqpComponents/src/AmqpConfiguration.h"
#include "Core/api/IConfigurable.h"
#include "Shared/tst/QTestConvenienceMacros.h"



using IpcAdapter::Components::AmqpComponents::AmqpConfigurableTest;
using IpcAdapter::Components::AmqpComponents::AmqpConfigurable;
using IpcAdapter::Components::AmqpComponents::AmqpConfiguration;
using IpcAdapter::Core::IConfigurable;



namespace
{
    void testConfiguration(
        std::function<void(IConfigurable&)> const& configurationCallback,
        bool expectConfigurationComplete,
        char const* const configurationCompleteErrorMessage
    )
    {
        TEST_REQUIREMENT("R-IPCA-AMQPCFG-001");

        AmqpConfiguration configuration;
        AmqpConfigurable configurable(configuration, []() {});

        configurable.onConfigureBegin();
        configurationCallback(configurable);
        COMPARE(configurable.onConfigureEnd(), expectConfigurationComplete, configurationCompleteErrorMessage);
    }
}



AmqpConfigurableTest::AmqpConfigurableTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
}



void AmqpConfigurableTest::test_01_configuring_unsupported_host_must_fail()
{
    testConfiguration([](IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("amqp.host", ""), false, "configuration fails for invalid 'amqp.host' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void AmqpConfigurableTest::test_02_configuring_unsupported_port_must_fail()
{
    for (auto const& portValue : std::initializer_list<QString> {"affe", "-1", "65537"})
    {
        testConfiguration([&portValue](IConfigurable & configurable)
        {
            COMPARE(configurable.doConfigure("amqp.port", portValue), false, "configuration fails for non int 'amqp.port' value");
        }, false, "configuration must fail in case faulty parameter was used");
    }
}



void AmqpConfigurableTest::test_03_configuring_unsupported_vhost_must_fail()
{
    testConfiguration([](IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("auth.vhost", ""), false, "configuration fails for empty 'auth.vhost' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void AmqpConfigurableTest::test_04_configuring_unsupported_exchange_type_must_fail()
{
    testConfiguration([](IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("exchange.type", ""), false, "configuration fails for unsupported 'exchange.type' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void AmqpConfigurableTest::test_05_configuring_unsupported_routing_key_must_fail()
{
    testConfiguration([](IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("exchange.routing-key", ""), false, "configuration fails for empty 'exchange.routingkey' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void AmqpConfigurableTest::test_06_configuring_unsupported_parameter_must_fail()
{
    testConfiguration([](IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("unknown", "value"), false, "using an unknown parameter must fail");
    }, true, "w/o configuring a known parameter wrongly we succeed");
}



void AmqpConfigurableTest::test_10_test_AmqpConfiguration_uri_getter()
{
    TEST_REQUIREMENT("R-IPCA-AMQPCFG-005");

    AmqpConfiguration configuration;
    configuration.host = QHostAddress::LocalHost;
    configuration.port = 12345;
    configuration.user = "user";
    configuration.pwd = "pwd";
    configuration.vhost = "vhost";

    COMPARE(configuration.getAmqpUri(AmqpConfiguration::UriStyle::ForUsage), QString("amqp://user:pwd@127.0.0.1:12345/vhost"), "ensure AMQP URI is complete 'for usage'");
    COMPARE(configuration.getAmqpUri(AmqpConfiguration::UriStyle::LogSafe), QString("amqp://user:***@127.0.0.1:12345/vhost"), "ensure AMQP URI is log-safe in 'log safe' mode");

    configuration.vhost = "/";
    COMPARE(configuration.getAmqpUri(AmqpConfiguration::UriStyle::ForUsage), QString("amqp://user:pwd@127.0.0.1:12345/"), "ensure AMQP URI does not carry duplicate '/'");
}
