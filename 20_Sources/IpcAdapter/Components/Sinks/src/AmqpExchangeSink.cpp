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
    namespace Constants
    {
        DECLARE_CONST_VARIADIC(QList<QString>, supportedExchangeTypes, {"topic", "direct", "fanout"});
    }

    struct AmqpConfiguration
    {
        QHostAddress host;
        quint16 port = 0;

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

#define PARAM_START(NAME) parameterHandler.insert(NAME, [](AmqpConfiguration & c, auto const & value)
#define PARAM_END )
#define PARAM_SETTER_ALWAYS(NAME, FIELD) PARAM_START(NAME) { c.FIELD = value; return true; } PARAM_END
#define PARAM_SETTER_VALIDATE(NAME, FIELD, VALIDATOR) PARAM_START(NAME) { c.FIELD = value; return VALIDATOR; } PARAM_END

        explicit AmqpConfigurable(AmqpConfiguration& aConfiguration, ConfigurationFinishedCallback const& aCallback)
            : configuration(aConfiguration)
            , callback(aCallback)
        {
            PARAM_SETTER_VALIDATE("auth.vhost", vhost, !value.isEmpty());
            PARAM_SETTER_ALWAYS("auth.user", user);
            PARAM_SETTER_ALWAYS("auth.pwd", pwd);
            PARAM_SETTER_ALWAYS("exchange.name", exchangeName);
            PARAM_SETTER_VALIDATE("exchange.routing-key", routingKey, !value.isEmpty());
            PARAM_SETTER_VALIDATE("exchange.type", exchangeType, Constants::supportedExchangeTypes().contains(value));
            PARAM_START("amqp.host")
            {
                return c.host.setAddress(value);
            }
            PARAM_END;
            PARAM_START("amqp.port")
            {
                auto conversionOk = false;
                auto const& port = value.toInt(&conversionOk);
                conversionOk = conversionOk &&
                               port >= std::numeric_limits<quint16>::min() && port <= std::numeric_limits<quint16>::max();

                c.port = static_cast<quint16>(port);
                return conversionOk;
            }
            PARAM_END;
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

        readyToUse = true;
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
    if (d->exchange != nullptr && d->readyToUse)
    {
        auto const& data = aPipelineFrame.getData();
        auto const& metaData = aPipelineFrame.getMetaData();

        QString routingKey = d->configuration.routingKey;

        REALIZE_REQUIREMENT("R-IPCA-SINK-008");
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

    return false;
}



void AmqpExchangeSink::setAmqpClient(AmqpClientPtr const& anAmqpClient)
{
    d->client = anAmqpClient;
}



AmqpExchangeSink::AmqpExchangeSink(): d(std::make_unique<Data>()) {}
AmqpExchangeSink::~AmqpExchangeSink() = default;



REGISTER_COMPONENT(AmqpExchangeSink)
