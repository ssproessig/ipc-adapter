#include "AmqpExchangeSinkTest.h"
#include "QAmqpSpyObjects.h"

#include "Components/AmqpComponents/src/AmqpExchangeSink.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/IConfigurable.h"
#include "Core/api/SimplePipelineFrame.h"
#include "Shared/tst/QTestConvenienceMacros.h"



using IpcAdapter::Components::AmqpComponents::AmqpExchangeSinkTest;
using IpcAdapter::Components::AmqpComponents::AmqpExchangeSink;
using IpcAdapter::Core::SimplePipelineFrame;



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
    TEST_REQUIREMENT("R-IPCA-AMQPCFG-002");
    TEST_REQUIREMENT("R-IPCA-AMQPCFG-003");

    auto const spy = std::make_shared<QAmqpClientSpy>();
    AmqpExchangeSink sink;
    sink.setAmqpClient(spy);

    auto configurable = sink.get();
    VERIFY(configurable != nullptr, "ensure we have a configurable");

    configurable->onConfigureBegin();
    configurable->onConfigureEnd();

    COMPARE(spy->uriSeen, QString("amqp://guest:guest@127.0.0.1:5672/"), "ensure default parameters have been used");

    TEST_REQUIREMENT("R-IPCA-AMQPCFG-004");
    emit spy->connected();
    QTest::qWait(50);

    VERIFY(spy->exchange.get() != nullptr, "ensure an exchange was created");
    COMPARE(spy->exchangeNameSeen, QString{}, "ensure default exchange '' was declared");
    COMPARE(spy->exchange->typeSeen, QString{"direct"}, "ensure default exchange type 'direct' was used");
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

    auto configurable = sink.get();
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
    TEST_REQUIREMENT("R-IPCA-AMQPSNK-002");

    auto const spy = std::make_shared<QAmqpClientSpy>();
    AmqpExchangeSink sink;
    sink.setAmqpClient(spy);

    auto configurable = sink.get();
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
