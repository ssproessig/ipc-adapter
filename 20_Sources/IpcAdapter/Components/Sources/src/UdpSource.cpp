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
        throw std::logic_error("The method or operation is not implemented.");
    }

    bool doConfigure(QString const& aKey, QString const& aValue) override
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

    bool onConfigureEnd() override
    {
        throw std::logic_error("The method or operation is not implemented.");
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


REGISTER_COMPONENT(UdpSource);
