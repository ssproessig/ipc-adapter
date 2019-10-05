#include "AmqpExchangeSinkTest.h"

#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/IConfigurable.h"
#include "Core/api/SimplePipelineFrame.h"
#include "Components/AmqpComponents/src/AmqpExchangeSink.h"
#include "Shared/tst/QTestConvenienceMacros.h"

#include "github.com.mbroadst/qamqp/qamqpclient.h"
#include "github.com.mbroadst/qamqp/qamqpexchange.h"


using IpcAdapter::Components::AmqpComponents::AmqpExchangeSinkTest;
using IpcAdapter::Components::AmqpComponents::AmqpExchangeSink;
using IpcAdapter::Core::IConfigurable;
using IpcAdapter::Core::SimplePipelineFrame;



namespace
{
    struct QAmqpExchangeSpy: QAmqpExchange
    {
        void declare(const QString& type, ExchangeOptions, const QAmqpTable&) override
        {
            typeSeen = type;
        }

        void publish(const QString& message, const QString& routingKey, const QAmqpMessage::PropertyHash&, int) override
        {
            messageSeen = message;
            routingKeySeen = routingKey;
        }

        QString typeSeen;
        QString messageSeen;
        QString routingKeySeen;
    };

    struct QAmqpClientSpy: QAmqpClient
    {
        void connectToHost(const QString& uri) override
        {
            uriSeen = uri;
        }

        QAmqpExchange* createExchange(const QString& name, int) override
        {
            exchangeNameSeen = name;
            exchange = std::make_shared<QAmqpExchangeSpy>();
            return exchange.get();
        }

        QString uriSeen;
        QString exchangeNameSeen;
        std::shared_ptr<QAmqpExchangeSpy> exchange;
    };

    void testConfiguration(
        std::shared_ptr<AmqpExchangeSink> const& uut,
        std::function<void(IConfigurable&)> configurationCallback,
        bool expectConfigurationComplete,
        char const* const configurationCompleteErrorMessage
    )
    {
        TEST_REQUIREMENT("R-IPCA-AMQPSNK-002");
        auto& configurable = *uut->getConfigurable();
        {
            configurable.onConfigureBegin();
            configurationCallback(configurable);
            COMPARE(configurable.onConfigureEnd(), expectConfigurationComplete, configurationCompleteErrorMessage);
        }
    }
}



AmqpExchangeSinkTest::AmqpExchangeSinkTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("R-IPCA-AMQPSNK-001");
}



void AmqpExchangeSinkTest::test_00_AmqpExchangeSink_is_registered_in_global_factory_registry()
{
    VERIFY(
        IpcAdapter::Core::GlobalComponentRegistry::get().getFactoryFor("AmqpExchangeSink") != nullptr,
        "Ensure the AmqpExchangeSink is published to global registry"
    );
}


void AmqpExchangeSinkTest::test_01_AmqpExchangeSink_default_parameters()
{
    TEST_REQUIREMENT("R-IPCA-AMQPSNK-003");
    TEST_REQUIREMENT("R-IPCA-AMQPSNK-004");

    auto const spy = std::make_shared<QAmqpClientSpy>();
    AmqpExchangeSink sink;
    sink.setAmqpClient(spy);

    auto configurable = sink.getConfigurable();
    VERIFY(configurable != nullptr, "ensure we have a configurable");

    configurable->onConfigureBegin();
    configurable->onConfigureEnd();

    COMPARE(spy->uriSeen, QString("amqp://guest:guest@127.0.0.1:5672/"), "ensure default parameters have been used");

    TEST_REQUIREMENT("R-IPCA-AMQPSNK-005");
    emit spy->connected();
    QTest::qWait(50);

    VERIFY(spy->exchange.get() != nullptr, "ensure an exchange was created");
    COMPARE(spy->exchangeNameSeen, QString{}, "ensure default exchange '' was declared");
    COMPARE(spy->exchange->typeSeen, QString{"direct"}, "ensure default exchange type 'direct' was used");
}



void AmqpExchangeSinkTest::test_02_AmqpExchangeSink_configuring_unsupported_host_must_fail()
{
    testConfiguration(std::make_shared<AmqpExchangeSink>(), [](IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("amqp.host", ""), false, "configuration fails for invalid 'amqp.host' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void AmqpExchangeSinkTest::test_03_AmqpExchangeSink_configuring_unsupported_port_must_fail()
{
    for (auto const& portValue : std::initializer_list<QString> {"affe", "-1", "65537"})
    {
        testConfiguration(std::make_shared<AmqpExchangeSink>(), [&portValue](IConfigurable & configurable)
        {
            COMPARE(configurable.doConfigure("amqp.port", portValue), false, "configuration fails for non int 'amqp.port' value");
        }, false, "configuration must fail in case faulty parameter was used");
    }
}



void AmqpExchangeSinkTest::test_04_AmqpExchangeSink_configuring_unsupported_vhost_must_fail()
{
    testConfiguration(std::make_shared<AmqpExchangeSink>(), [](IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("auth.vhost", ""), false, "configuration fails for empty 'auth.vhost' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void AmqpExchangeSinkTest::test_05_AmqpExchangeSink_configuring_unsupported_exchange_type_must_fail()
{
    testConfiguration(std::make_shared<AmqpExchangeSink>(), [](IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("exchange.type", ""), false, "configuration fails for unsupported 'exchange.type' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void AmqpExchangeSinkTest::test_06_AmqpExchangeSink_configuring_unsupported_routing_key_must_fail()
{
    testConfiguration(std::make_shared<AmqpExchangeSink>(), [](IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("exchange.routing-key", ""), false, "configuration fails for empty 'exchange.routingkey' value");
    }, false, "configuration must fail in case faulty parameter was used");
}



void AmqpExchangeSinkTest::test_07_AmqpExchangeSink_configuring_unsupported_parameter_must_fail()
{
    testConfiguration(std::make_shared<AmqpExchangeSink>(), [](IConfigurable & configurable)
    {
        COMPARE(configurable.doConfigure("unknown", "value"), false, "using an unknown parameter must fail");
    }, true, "w/o configuring a known parameter wrongly we succeed");
}



void AmqpExchangeSinkTest::test_20_sending_must_fail_if_not_configured()
{
    AmqpExchangeSink sink;
    SimplePipelineFrame const frame;

    COMPARE(sink.process(frame), false, "sending must fail if not configured");
}



void AmqpExchangeSinkTest::test_21_sending_with_default_parameters_succeeds()
{
    auto const spy = std::make_shared<QAmqpClientSpy>();
    AmqpExchangeSink sink;
    sink.setAmqpClient(spy);

    auto configurable = sink.getConfigurable();
    VERIFY(configurable != nullptr, "ensure we have a configurable");

    configurable->onConfigureBegin();
    configurable->onConfigureEnd();
    emit spy->connected();
    QTest::qWait(50);

    SimplePipelineFrame const frame{"data"};
    COMPARE(sink.process(frame), true, "sending must succeed with default configuration");

    COMPARE(spy->exchange->messageSeen, QString("data"), "we received 'data'");
    COMPARE(spy->exchange->routingKeySeen, QString(""), "we used an empty routing-key per default");
}



void AmqpExchangeSinkTest::test_22_sending_with_routing_key_replacement_works()
{
    TEST_REQUIREMENT("R-IPCA-AMQPSNK-006");

    auto const spy = std::make_shared<QAmqpClientSpy>();
    AmqpExchangeSink sink;
    sink.setAmqpClient(spy);

    auto configurable = sink.getConfigurable();
    VERIFY(configurable != nullptr, "ensure we have a configurable");

    configurable->onConfigureBegin();
    configurable->doConfigure("exchange.routing-key", "abc.${key}.xyz");
    configurable->onConfigureEnd();
    emit spy->connected();
    QTest::qWait(50);

    SimplePipelineFrame frame{"data"};
    frame.updateMetaData("key", "value123");
    COMPARE(sink.process(frame), true, "sending must succeed with default configuration");

    COMPARE(spy->exchange->routingKeySeen, QString("abc.value123.xyz"), "we used an empty routing-key per default");
}
