#include "Core/src/RuntimeConfiguration.h"
#include "Core/api/IComponent.h"
#include "Core/api/Logger.h"

#include <QMap>
#include <QString>



using IpcAdapter::Core::RuntimeConfiguration;



struct RuntimeConfiguration::Data
{
    ComponentMap components;
};



RuntimeConfiguration::RuntimeConfiguration() : d(std::make_unique<Data>()) {}
RuntimeConfiguration::~RuntimeConfiguration() = default;



IpcAdapter::Core::ComponentMap const& RuntimeConfiguration::getComponents() const
{
    return d->components;
}



bool RuntimeConfiguration::containsComponentCalled(QString const& aComponentId)
{
    return d->components.contains(aComponentId);
}



void RuntimeConfiguration::addComponent(QString const& aComponentId, IpcAdapter::Core::ComponentPtr const& aComponent)
{
    LOG_DEBUG(this) << "added component " << aComponentId << "=" << aComponent.get();
    d->components.insert(aComponentId, aComponent);
}
