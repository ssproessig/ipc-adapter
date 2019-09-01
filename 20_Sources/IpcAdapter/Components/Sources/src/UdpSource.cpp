#include "UdpSource.h"

#include "Core/api/IConfigurable.h"
#include "Core/api/GlobalComponentRegistry.h"

#include <QString>



using IpcAdapter::Components::Sources::UdpSource;



struct UdpSource::Data: Core::IConfigurable
{
    /// @name IConfigurable implementation
    /// @{
    void onConfigureBegin() override
    {
    }

    bool doConfigure(QString const& aKey, QString const& aValue) override
    {
        return false;
    }

    bool onConfigureEnd() override
    {
        return false;
    }
    /// @}
};



UdpSource::UdpSource()
    : d(std::make_unique<Data>())
{
    REALIZE_REQUIREMENT("R-IPCA-SOURCE-001");
}
UdpSource::~UdpSource() = default;



IpcAdapter::Core::IConfigurable& UdpSource::getConfigurable()
{
    return *d;
}


void UdpSource::sourceTo(IpcAdapter::Core::PipelineStepPtr const& aPipelineStep)
{
}



REGISTER_COMPONENT(UdpSource);
