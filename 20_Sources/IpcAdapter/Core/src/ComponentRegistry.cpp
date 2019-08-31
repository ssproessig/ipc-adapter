#include "Core/src/ComponentRegistry.h"



using IpcAdapter::Core::ComponentRegistry;



struct ComponentRegistry::Data {};



IpcAdapter::Core::IFactoryRegistry::FactoryFnc
ComponentRegistry::getFactoryFor(QString const& aComponentId)
{
    return nullptr;
}



ComponentRegistry::ComponentRegistry(): d(std::make_unique<Data>()) {}
ComponentRegistry::~ComponentRegistry() = default;
