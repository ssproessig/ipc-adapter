#include "UdpSink.h"

#include "Core/api/IConfigurable.h"
#include "Core/api/IPipelineStep.h"



using IpcAdapter::Components::Sinks::UdpSink;



struct UdpSink::Data
    : IpcAdapter::Core::IConfigurable
{

    void onConfigureBegin() override {}

    bool doConfigure(QString const& aKey, QString const& aValue) override
    {
        return false;
    }

    bool onConfigureEnd() override
    {
        return false;
    }
};



UdpSink::UdpSink()
    : d(std::make_unique<Data>())
{}

UdpSink::~UdpSink() = default;


IpcAdapter::Core::IConfigurable& UdpSink::getConfigurable()
{
    return *d;
}


void UdpSink::forwardTo(IpcAdapter::Core::IPipelineStep& aPipelineStep)
{
}
