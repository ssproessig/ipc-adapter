#include "AmqpExchangeSink.h"

#include "Core/api/IConfigurable.h"
#include "Core/api/IPipelineFrame.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/Logger.h"

#include "github.com.mbroadst/qamqp/qamqpclient.h"
#include "github.com.mbroadst/qamqp/qamqpexchange.h"

#include <QHostAddress>
#include <QString>



using IpcAdapter::Components::Sinks::AmqpExchangeSink;
using IpcAdapter::Core::IPipelineFrame;
using AmqpExchangePtr = std::shared_ptr<QAmqpExchange>;



namespace
{
    struct AmqpConfiguration
    {
        QHostAddress host;
        quint16 port = -1;

        QString user;
        QString pwd;
        QString vhost;

        QString exchangeName;
        QString exchangeType;

        QString routingKey;

        AmqpConfiguration()
        {
            initDefaults();
        }

        void initDefaults()
        {
            REALIZE_REQUIREMENT("R-IPCA-SINK-005");

            host = QHostAddress("127.0.0.1");
            port = 5672;

            REALIZE_REQUIREMENT("R-IPCA-SINK-006");
            user = "guest";
            pwd = "guest";
            vhost = "/";

            REALIZE_REQUIREMENT("R-IPCA-SINK-007");
            exchangeName.clear();
            exchangeType = "direct";

            routingKey.clear();
        }

        enum class UriStyle : bool
        {
            ForUsage,
            LogSafe
        };

        QString getAmqpUri(UriStyle const anUriStyle) const
        {
            auto const password = anUriStyle == UriStyle::ForUsage ? pwd : "***";

            return QStringLiteral("amqp://%1:%2@%3:%4%5")
                   .arg(user).arg(password).arg(host.toString()).arg(port).arg(vhost);
        }
    };


    using ConfigurationFinishedCallback = std::function<void()>;


    struct AmqpConfigurable: IpcAdapter::Core::IConfigurable
    {
        explicit AmqpConfigurable(AmqpConfiguration& aConfiguration, ConfigurationFinishedCallback const& aCallback)
            : configuration(aConfiguration)
            , callback(aCallback)
        {}
        ~AmqpConfigurable() override = default;


        void onConfigureBegin() override
        {
            // disconnectFromHost
            configuration.initDefaults();
        }

        bool doConfigure(QString const& aKey, QString const& aValue) override
        {
            // parse host, port, vhost, user, pwd
            // parse exchange name and type

            return false;
        }

        bool onConfigureEnd() override
        {
            auto const succeeded = true; // FIXME

            if (succeeded)
            {
                callback();
            }

            return succeeded;
        }

        AmqpConfiguration& configuration;
        ConfigurationFinishedCallback callback;
    };
}



struct AmqpExchangeSink::Data : QObject
{
    Data(): QObject() {}

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
    }

    AmqpConfiguration configuration;
    AmqpConfigurable configurable{configuration, std::bind(&Data::amqpSetup, this)};

    AmqpClientPtr client = std::make_shared<QAmqpClient>();
    QAmqpExchange* exchange = nullptr;

    bool readyToUse = false;
};


IpcAdapter::Core::IConfigurable* AmqpExchangeSink::getConfigurable()
{
    return &d->configurable;
}



bool AmqpExchangeSink::process(IPipelineFrame const& aPipelineFrame)
{
    return false;
}



void AmqpExchangeSink::setAmqpClient(AmqpClientPtr const& anAmqpClient)
{
    d->client = anAmqpClient;
}



AmqpExchangeSink::AmqpExchangeSink(): d(std::make_unique<Data>()) {}
AmqpExchangeSink::~AmqpExchangeSink() = default;



REGISTER_COMPONENT(AmqpExchangeSink)
