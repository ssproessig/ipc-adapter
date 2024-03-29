#include "UdpSource.h"

#include "Core/api/IConfigurable.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/Logger.h"
#include "Core/api/SimplePipelineFrame.h"

#include <QHostAddress>
#include <QNetworkDatagram>
#include <QString>
#include <QUdpSocket>
#include <QVariant>



using IpcAdapter::Components::NetworkComponents::UdpSource;



namespace
{
    namespace Constants
    {
        DECLARE_CONST(QString, configHost, QStringLiteral("host"));
        DECLARE_CONST(QString, configPort, QStringLiteral("port"));
    }

    namespace Defaults
    {
        DECLARE_CONST(QHostAddress, host, ("127.0.0.1"));
        DECLARE_CONST(quint16, port, (6789));
    }
}



struct UdpSource::Data: Core::IConfigurable
{
    Data()
    {
        initializeDefaults();

        QObject::connect(&socket, &QUdpSocket::readyRead, [this]()
        {
            auto const dataReceived = socket.receiveDatagram();
            LOG_DEBUG(this) << "received: " << dataReceived.data().toHex();

            EXIT_EARLY_IF(Q_UNLIKELY(!sourceTo),);

            auto const pipelineFrame = std::make_shared<Core::SimplePipelineFrame>(dataReceived.data());

            REALIZE_REQUIREMENT("R-IPCA-UDPSRC-004");
            pipelineFrame->updateMetaData("UdpSource:host", dataReceived.senderAddress().toString());
            pipelineFrame->updateMetaData("UdpSource:port", dataReceived.senderPort());

            LOG_DEBUG(this) << "forwarding to " << sourceTo << " " << pipelineFrame->getData();

            sourceTo->process(pipelineFrame);
        });
    }

    void initializeDefaults()
    {
        REALIZE_REQUIREMENT("R-IPCA-UDPSRC-003");

        hasValidHost = true;
        hasValidPort = true;
        targetHost = Defaults::host();
        targetPort = Defaults::port();
    }

    /// @name IConfigurable implementation
    /// @{
    void onConfigureBegin() override
    {
        initializeDefaults();
    }

    bool doConfigure(QString const& aKey, QString const& aValue) override
    {
        REALIZE_REQUIREMENT("R-IPCA-UDPSRC-002");

        LOG_DEBUG(this) << "doConfigure: " << aKey << ", " << aValue;

        if (aKey == Constants::configHost())
        {
            hasValidHost = targetHost.setAddress(aValue);
            return hasValidHost;
        }

        if (aKey == Constants::configPort())
        {
            bool conversionOk = false;
            auto const& port = aValue.toInt(&conversionOk);
            hasValidPort = conversionOk &&
                           port >= std::numeric_limits<quint16>::min() && port <= std::numeric_limits<quint16>::max();

            targetPort = static_cast<quint16>(port);

            return hasValidPort;
        }

        return false;
    }

    bool onConfigureEnd() override
    {
        LOG_DEBUG(this) << "hasValidHost=" << hasValidHost << " hasValidPort=" << hasValidPort;
        configuredOk = hasValidHost && hasValidPort;

        if (configuredOk)
        {
            configuredOk = socket.bind(targetHost, targetPort);

            if (!configuredOk)
            {
                LOG_ERROR(this) << "Binding failed: " << targetHost << ":" << targetPort;
            }
        }

        return configuredOk;
    }
    /// @}


    QHostAddress targetHost = QHostAddress::LocalHost;
    quint16 targetPort = 0;

    Core::IPipelineStep* sourceTo = nullptr;
    QUdpSocket socket;

    bool configuredOk = false;
    bool hasValidHost = false;
    bool hasValidPort = false;
};



UdpSource::UdpSource()
    : d(std::make_unique<Data>())
{
    REALIZE_REQUIREMENT("R-IPCA-UDPSRC-001");
}
UdpSource::~UdpSource() = default;



IpcAdapter::Core::IConfigurable* UdpSource::get() const
{
    return d.get();
}



void UdpSource::sourceTo(IpcAdapter::Core::IPipelineStep* aPipelineStep)
{
    d->sourceTo = aPipelineStep;
}



REGISTER_COMPONENT(UdpSource)
