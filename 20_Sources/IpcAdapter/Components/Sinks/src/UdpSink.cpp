#include "UdpSink.h"

#include "Core/api/IConfigurable.h"
#include "Core/api/IPipelineFrame.h"

#include <QHostAddress>
#include <QString>



using IpcAdapter::Components::Sinks::UdpSink;



namespace
{
    namespace Constants
    {
        DECLARE_CONST(QString, configHost, QStringLiteral("host"));
    }
}



struct UdpSink::Data
    : IpcAdapter::Core::IConfigurable
{
    /// @name IConfigurable implementation
    /// @{
    void onConfigureBegin() override {}

    bool doConfigure(QString const& aKey, QString const& aValue) override
    {
        REALIZE_REQUIREMENT("R-IPCA-SINK-002");

        if(aKey == Constants::configHost())
        {

        }
        else
        {
            return false;
        }

        return true;
    }

    bool onConfigureEnd() override
    {
        return false;
    }
    /// @}


    QHostAddress targetHost = QHostAddress::LocalHost;
    quint16 port = 0;
};



UdpSink::UdpSink()
    : d(std::make_unique<Data>())
{
    REALIZE_REQUIREMENT("R-IPCA-SINK-001");
}

UdpSink::~UdpSink() = default;


IpcAdapter::Core::IConfigurable& UdpSink::getConfigurable()
{
    return *d;
}


bool UdpSink::process(IpcAdapter::Core::IPipelineFrame& aPipelineStep)
{
    return false;
}
