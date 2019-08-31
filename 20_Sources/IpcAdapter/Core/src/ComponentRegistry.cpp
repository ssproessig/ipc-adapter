#include "Core/src/ComponentRegistry.h"

#include "Core/api/Logger.h"

#include <QMap>



using IpcAdapter::Core::ComponentRegistry;
using FactoryLookupTable = QMap<QString, IpcAdapter::Core::IFactoryRegistry::FactoryFnc>;



struct ComponentRegistry::Data
{
    FactoryLookupTable knownFactories;
};



IpcAdapter::Core::IFactoryRegistry::FactoryFnc
ComponentRegistry::getFactoryFor(QString const& aComponentId)
{
    auto const& it = d->knownFactories.constFind(aComponentId);

    if(it != d->knownFactories.constEnd())
    {
        LOG_DEBUG(this) << "Found factory for" <<  aComponentId;
        return *it;
    }

    LOG_DEBUG(this) << "Component" << aComponentId << "is unknown!";
    return nullptr;
}


bool ComponentRegistry::registerFactoryFor(QString const& aComponentId, FactoryFnc const& aFactoryFunction)
{
    auto const& it = d->knownFactories.constFind(aComponentId);

    if(it == d->knownFactories.constEnd())
    {
        LOG_DEBUG(this) << "Added " << aComponentId << " to registry";
        return d->knownFactories.insert(aComponentId, aFactoryFunction) != d->knownFactories.constEnd();
    }

    LOG_DEBUG(this) << "Refusing to add " << aComponentId << " twice!";
    return false;
}


ComponentRegistry::ComponentRegistry(): d(std::make_unique<Data>()) {}
ComponentRegistry::~ComponentRegistry() = default;
