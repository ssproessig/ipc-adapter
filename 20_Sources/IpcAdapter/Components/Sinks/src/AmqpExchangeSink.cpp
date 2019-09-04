#include "AmqpExchangeSink.h"

#include "Core/api/IConfigurable.h"
#include "Core/api/IPipelineFrame.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/Logger.h"

#include "github.com.mbroadst/qamqp/qamqpclient.h"

#include <QHostAddress>
#include <QString>



using IpcAdapter::Components::Sinks::AmqpExchangeSink;
using IpcAdapter::Core::IPipelineFrame;



namespace
{
    struct AmqpConfiguration
    {
        QHostAddress host;
        quint16 port = -1;

        QString user;
        QString pwd;
        QString vhost;

        AmqpConfiguration()
        {
            initDefaults();
        }

        void initDefaults()
        {
            REALIZE_REQUIREMENT("R-IPCA-SINK-005");
            REALIZE_REQUIREMENT("R-IPCA-SINK-006");

            host = QHostAddress("127.0.0.1");
            port = 5672;
            user = "guest";
            pwd = "guest";
            vhost = "/";
        }

        QString getAmqpUri() const
        {
            return QStringLiteral("amqp://%1:%2@%3:%4%5")
                   .arg(user).arg(pwd).arg(host.toString()).arg(port).arg(vhost);
        }
    };

    struct AmqpConfigurable: IpcAdapter::Core::IConfigurable
    {
        NONCOPYABLE(AmqpConfigurable);
        explicit AmqpConfigurable(AmqpClientPtr& aClient, AmqpConfiguration& aConfiguration)
            : client(aClient)
            , configuration(aConfiguration)
        {}
        ~AmqpConfigurable() override = default;


        void onConfigureBegin() override
        {
            configuration.initDefaults();
        }

        bool doConfigure(QString const& aKey, QString const& aValue) override
        {
            return false;
        }

        bool onConfigureEnd() override
        {
            client->connectToHost(configuration.getAmqpUri());
            return false;
        }

        AmqpClientPtr& client;
        AmqpConfiguration& configuration;
    };
}



struct AmqpExchangeSink::Data
{
    AmqpClientPtr client = std::make_shared<QAmqpClient>();
    AmqpConfiguration configuration;
    AmqpConfigurable configurable{client, configuration};
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
