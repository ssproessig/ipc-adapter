#include "Core/src/ComponentRegistry.h"

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
        return *it;
    }

    return nullptr;
}


bool ComponentRegistry::addFactoryFor(QString const& aComponentId, FactoryFnc const& aFactoryFunction)
{
    auto const& it = d->knownFactories.constFind(aComponentId);

    if(it == d->knownFactories.constEnd())
    {
        return d->knownFactories.insert(aComponentId, aFactoryFunction) != d->knownFactories.constEnd();
    }

    return false;
}


ComponentRegistry::ComponentRegistry(): d(std::make_unique<Data>()) {}
ComponentRegistry::~ComponentRegistry() = default;
