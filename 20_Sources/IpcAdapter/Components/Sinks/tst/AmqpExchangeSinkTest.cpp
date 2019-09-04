#include "AmqpExchangeSinkTest.h"

#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/IConfigurable.h"
#include "Components/Sinks/src/AmqpExchangeSink.h"
#include "Shared/tst/QTestConvenienceMacros.h"

#include "github.com.mbroadst/qamqp/qamqpclient.h"
#include "github.com.mbroadst/qamqp/qamqpexchange.h"



using IpcAdapter::Components::Sinks::AmqpExchangeSinkTest;
using IpcAdapter::Components::Sinks::AmqpExchangeSink;



namespace
{
    struct QAmqpExchangeSpy: QAmqpExchange
    {
        void declare(const QString& type, ExchangeOptions options, const QAmqpTable& args) override
        {
            typeSeen = type;
        }

        QString typeSeen;
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
}



AmqpExchangeSinkTest::AmqpExchangeSinkTest()
{
    TEST_SPEC_META_INFORMATION("Soeren Sproessig");
    TEST_REQUIREMENT("R-IPCA-SINK-003");
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
    TEST_REQUIREMENT("R-IPCA-SINK-005");
    TEST_REQUIREMENT("R-IPCA-SINK-006");

    auto const spy = std::make_shared<QAmqpClientSpy>();
    AmqpExchangeSink sink;
    sink.setAmqpClient(spy);

    auto configurable = sink.getConfigurable();
    VERIFY(configurable != nullptr, "ensure we have a configurable");

    configurable->onConfigureBegin();
    configurable->onConfigureEnd();

    COMPARE(spy->uriSeen, QString("amqp://guest:guest@127.0.0.1:5672/"), "ensure default parameters have been used");

    TEST_REQUIREMENT("R-IPCA-SINK-007");
    emit spy->connected();
    QTest::qWait(50);

    VERIFY(spy->exchange.get() != nullptr, "ensure an exchange was created");
    COMPARE(spy->exchangeNameSeen, QString{}, "ensure default exchange '' was declared");
    COMPARE(spy->exchange->typeSeen, QString{"direct"}, "ensure default exchange type 'direct' was used");
}
