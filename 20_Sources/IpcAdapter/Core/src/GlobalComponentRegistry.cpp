#include "Core/api/GlobalComponentRegistry.h"
#include "Core/src/ComponentRegistry.h"



namespace
{
    DECLARE_SINGLETON(IpcAdapter::Core::ComponentRegistry, singletonRegistry,);
}



IpcAdapter::Core::IFactoryRegistry& IpcAdapter::Core::GlobalComponentRegistry::get()
{
    return ::singletonRegistry();
}
