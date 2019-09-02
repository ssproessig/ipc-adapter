#include "UdpSink.h"

#include "Core/api/IConfigurable.h"
#include "Core/api/IPipelineFrame.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/Logger.h"

#include <QHostAddress>
#include <QString>
#include <QUdpSocket>


using IpcAdapter::Components::Sinks::UdpSink;



namespace
{
    namespace Constants
    {
        DECLARE_CONST(QString, configHost, QStringLiteral("host"));
        DECLARE_CONST(QString, configPort, QStringLiteral("port"));
    }
}



struct UdpSink::Data
    : IpcAdapter::Core::IConfigurable
{
    /// @name IConfigurable implementation
    /// @{
    void onConfigureBegin() override
    {
        hasValidHost = false;
        hasValidPort = false;
    }

    bool doConfigure(QString const& aKey, QString const& aValue) override
    {
        REALIZE_REQUIREMENT("R-IPCA-SINK-002");

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
        return configuredOk;
    }
    /// @}


    QHostAddress targetHost = QHostAddress::LocalHost;
    quint16 targetPort = 0;

    std::unique_ptr<QUdpSocket> socket = std::make_unique<QUdpSocket>();

    bool configuredOk = false;
    bool hasValidHost = false;
    bool hasValidPort = false;
};



UdpSink::UdpSink()
    : d(std::make_unique<Data>())
{
    REALIZE_REQUIREMENT("R-IPCA-SINK-001");
}

UdpSink::~UdpSink() = default;


IpcAdapter::Core::IConfigurable* UdpSink::getConfigurable()
{
    return d.get();
}


bool UdpSink::process(IpcAdapter::Core::IPipelineFrame const& aPipelineStep)
{
    if (d->configuredOk)
    {
        auto const& data = aPipelineStep.getData();
        auto const bytesSent = d->socket->writeDatagram(data, d->targetHost, d->targetPort);

        LOG_DEBUG(this) << "send" << data << "to" << d->targetHost << ":" << d->targetPort;

        return bytesSent == data.length();
    }

    return false;
}



REGISTER_COMPONENT(UdpSink);

