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

        bool isOk = false;


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

            isOk = true;
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
        using SupportedParameters = QMap<QString, std::function<bool(AmqpConfiguration&, QString const&)>>;

        explicit AmqpConfigurable(AmqpConfiguration& aConfiguration, ConfigurationFinishedCallback const& aCallback)
            : configuration(aConfiguration)
            , callback(aCallback)
        {
            parameterHandler.insert("amqp.host", [](AmqpConfiguration & c, auto const & value)
            {
                return c.host.setAddress(value);
            });
            parameterHandler.insert("amqp.port", [](AmqpConfiguration & c, auto const & value)
            {
                auto conversionOk = false;
                auto const& port = value.toInt(&conversionOk);
                conversionOk = conversionOk &&
                               port >= std::numeric_limits<quint16>::min() && port <= std::numeric_limits<quint16>::max();

                c.port = static_cast<quint16>(port);
                return conversionOk;
            });
            parameterHandler.insert("auth.vhost", [](AmqpConfiguration & c, auto const & value)
            {
                c.vhost = value;
                return !value.isEmpty();
            });
            // parse host, port, vhost, user, pwd
            // parse exchange name and type

        }
        ~AmqpConfigurable() override = default;


        void onConfigureBegin() override
        {
            // FIXME: disconnectFromHost
            configuration.initDefaults();
        }

        bool doConfigure(QString const& aKey, QString const& aValue) override
        {
            LOG_DEBUG(this) << "configuring " << aKey << "to" << aValue;

            auto const& it = parameterHandler.constFind(aKey);

            if (it != parameterHandler.constEnd())
            {
                auto const configured = it.value()(configuration, aValue);
                LOG_DEBUG(this) << "result" << configured;

                if (!configured)
                {
                    configuration.isOk = false;
                }

                return configured;
            }

            LOG_ERROR(this) << "unsupported parameter: " << aKey;
            return false;
        }

        bool onConfigureEnd() override
        {
            if (configuration.isOk)
            {
                callback();
            }

            return configuration.isOk;
        }

        AmqpConfiguration& configuration;
        ConfigurationFinishedCallback callback;
        SupportedParameters parameterHandler;
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
