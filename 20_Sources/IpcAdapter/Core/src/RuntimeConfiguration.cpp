#include "Core/src/RuntimeConfiguration.h"
#include "Core/api/IComponent.h"
#include "Core/api/IPipeline.h"
#include "Core/api/ISource.h"
#include "Core/api/Logger.h"

#include <QMap>
#include <QString>



using IpcAdapter::Core::RuntimeConfiguration;
using IpcAdapter::Core::IPipelineStep;



namespace
{
    namespace Constants
    {
        DECLARE_CONST(QString, errorUnknownSource, ("trying to multiplex unknown source '%1'!"))
        DECLARE_CONST(QString, errorDuplicatePipelineId, ("trying to redefine pipeline '%1'!"))
    }

    struct SourceMultiplex
        : IpcAdapter::Core::ISource
        , IPipelineStep
    {
        IpcAdapter::Core::IConfigurable* getConfigurable() override
        {
            return nullptr;
        }

        void sourceTo(IPipelineStep* aPipelineStep) override
        {
            targets.append(aPipelineStep);
        }

        bool process(IpcAdapter::Core::PipelineFramePtr const& aPipelineFrame) override
        {
            for (auto const& target : targets)
            {
                target->process(aPipelineFrame);
            }

            return true;
        }

        QList<IPipelineStep*> targets;
    };
}



struct RuntimeConfiguration::Data
{
    QMap<QString, std::shared_ptr<IpcAdapter::Core::ISource>> sourceMultiplexes;

    ComponentMap components;
    PipelineMap pipelines;
};



RuntimeConfiguration::RuntimeConfiguration() : d(std::make_unique<Data>()) {}
RuntimeConfiguration::~RuntimeConfiguration() = default;



IpcAdapter::Core::ComponentMap const& RuntimeConfiguration::getComponents() const
{
    return d->components;
}



IpcAdapter::Core::ComponentPtr RuntimeConfiguration::getComponent(QString const& aComponentId) const
{
    auto const& it = d->components.constFind(aComponentId);

    return (it != d->components.constEnd()) ? *it : nullptr;
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



IpcAdapter::Core::ISource* RuntimeConfiguration::getSourceMultiplexFor(QString const& aSourceId)
{
    auto const& multiplexIt = d->sourceMultiplexes.constFind(aSourceId);

    if (multiplexIt != d->sourceMultiplexes.constEnd())
    {
        return multiplexIt.value().get();
    }

    auto const& sourceIt = d->components.constFind(aSourceId);

    if (sourceIt != d->components.constEnd())
    {
        auto asSource = std::dynamic_pointer_cast<ISource>(*sourceIt);

        if (asSource)
        {
            auto const multiplex = std::make_shared<SourceMultiplex>();
            d->sourceMultiplexes.insert(aSourceId, multiplex);

            auto const multiplexPtr = multiplex.get();
            asSource->sourceTo(multiplexPtr);
            return multiplexPtr;
        }
    }

    THROW(Constants::errorUnknownSource().arg(aSourceId));
}


void RuntimeConfiguration::addPipeline(QString const& aPipelineId, IpcAdapter::Core::PipelinePtr const& aPipeline)
{
    if (d->pipelines.contains(aPipelineId))
    {
        THROW(Constants::errorDuplicatePipelineId().arg(aPipelineId));
    }

    LOG_DEBUG(this) << "added pipeline " << aPipelineId << "=" << aPipeline.get();
    d->pipelines.insert(aPipelineId, aPipeline);
}



IpcAdapter::Core::PipelineMap const& RuntimeConfiguration::getPipelines() const
{
    return d->pipelines;
}
