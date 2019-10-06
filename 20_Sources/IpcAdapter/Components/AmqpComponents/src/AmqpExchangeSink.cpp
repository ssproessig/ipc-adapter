#include "AmqpExchangeSink.h"
#include "AmqpConfigurable.h"
#include "AmqpConfiguration.h"

#include "Core/api/IConfigurable.h"
#include "Core/api/IPipelineFrame.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/Logger.h"

#include "github.com.mbroadst/qamqp/qamqpclient.h"
#include "github.com.mbroadst/qamqp/qamqpexchange.h"

#include <QString>



using IpcAdapter::Components::AmqpComponents::AmqpConfigurable;
using IpcAdapter::Components::AmqpComponents::AmqpConfiguration;
using IpcAdapter::Components::AmqpComponents::AmqpExchangeSink;
using IpcAdapter::Core::IPipelineFrame;
using AmqpExchangePtr = std::shared_ptr<QAmqpExchange>;



struct AmqpExchangeSink::Data : QObject
{
    void amqpSetup()
    {
        readyToUse = false;
        connect(client.get(), &QAmqpClient::connected, this, &Data::amqpClientConnected);
        LOG_DEBUG(this) << "Connecting to " << configuration.getAmqpUri(AmqpConfiguration::UriStyle::LogSafe);
        client->connectToHost(configuration.getAmqpUri(AmqpConfiguration::UriStyle::ForUsage));
    }

    void amqpClientConnected()
    {
        LOG_DEBUG(this) << "...connected.";
        LOG_DEBUG(this) << "Declaring exchange " << configuration.exchangeName << "of type" << configuration.exchangeType;

        exchange = client->createExchange(configuration.exchangeName);
        exchange->declare(configuration.exchangeType);

        readyToUse = true;
    }

    AmqpConfiguration configuration;
    AmqpConfigurable configurable{configuration, std::bind(&Data::amqpSetup, this)};

    AmqpClientPtr client = std::make_shared<QAmqpClient>();
    QAmqpExchange* exchange = nullptr;

    bool readyToUse = false;
};


IpcAdapter::Core::IConfigurable* AmqpExchangeSink::get() const
{
    return &d->configurable;
}



bool AmqpExchangeSink::process(IPipelineFrame const& aPipelineFrame)
{
    if (d->exchange != nullptr && d->readyToUse)
    {
        auto const& data = aPipelineFrame.getData();
        auto const& metaData = aPipelineFrame.getMetaData();

        QString routingKey = d->configuration.routingKey;

        REALIZE_REQUIREMENT("R-IPCA-AMQPSNK-002");
        {
            for (auto const& key : metaData.keys())
            {
                auto const varName = QString("${%1}").arg(key);
                routingKey = routingKey.replace(varName, metaData.value(key).toString());
            }
        }

        d->exchange->publish(data, routingKey);

        LOG_DEBUG(this) << "published to exchange" << d->configuration.exchangeName << "with routing-key" << routingKey << ":" << data;
        return true;
    }

    LOG_WARN(this) << "not publishing as component is not configured correctly";
    return false;
}



void AmqpExchangeSink::setAmqpClient(AmqpClientPtr const& anAmqpClient)
{
    d->client = anAmqpClient;
}



AmqpExchangeSink::AmqpExchangeSink(): d(std::make_unique<Data>())
{
    REALIZE_REQUIREMENT("R-IPCA-AMQPSNK-001");
}
AmqpExchangeSink::~AmqpExchangeSink() = default;



REGISTER_COMPONENT(AmqpExchangeSink)
