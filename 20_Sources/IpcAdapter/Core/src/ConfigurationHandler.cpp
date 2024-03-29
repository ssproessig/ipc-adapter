#include "ConfigurationHandler.h"

#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/IComponent.h"
#include "Core/api/IConfigurable.h"
#include "Core/api/IConfigurator.h"
#include "Core/api/IConverter.h"
#include "Core/api/IProvider.h"
#include "Core/api/ISink.h"
#include "Core/api/ISource.h"
#include "Core/api/Logger.h"
#include "Core/src/Pipeline.h"
#include "Core/src/RuntimeConfiguration.h"

#include <QMap>
#include <QStack>
#include <QString>
#include <QXmlParseException>



using IpcAdapter::Core::ConfigurationHandler;
using IpcAdapter::Core::IComponent;
using IpcAdapter::Core::IConfigurable;
using IpcAdapter::Core::RuntimeConfiguration;
using HandlerStack = QStack<std::shared_ptr<QXmlDefaultHandler>>;



namespace
{
    namespace Constants
    {
        DECLARE_CONST(QString, supportedNamespace, ("urn:ipca-configuration-v1"))
        DECLARE_CONST(QString, errorUnsupportedNamespace, ("configuration has unsupported namespace '%1'!"))
        DECLARE_CONST(QString, errorUnsupportedRootElement, ("configuration has wrong root element '%1'!"))
        DECLARE_CONST(QString, errorWrongRootElementVersion, ("configuration has unsupported version '%1'!"))
        DECLARE_CONST(QString, errorUnknownComponent, ("unknown component '%1' requested!"))
        DECLARE_CONST(QString, errorDuplicateComponentId, ("unable to create '%1' again for id '%2'!"))
        DECLARE_CONST(QString, errorParamRejected, ("component '%1' rejects parameter '%2' with value '%3'!"))
        DECLARE_CONST(QString, errorFinishingConfiguration, ("unable to finish configuring '%1'!"))
        DECLARE_CONST(QString, errorWrongComponentType, ("component '%1' referenced in '%2' is no %3!"))
        DECLARE_CONST(QString, errorUnknownParamList, ("component '%1' references unknown param-list '%2'!"))
    }


    struct ConfigurableInContext
        : IConfigurable
    {
        QString forId;
        IConfigurable* c;

        explicit ConfigurableInContext(std::shared_ptr<IComponent> const& aComponent, QString const& anId)
            : forId(anId)
        {
            auto const provider = std::dynamic_pointer_cast<IpcAdapter::Core::IProvider<IConfigurable>>(aComponent);
            c = provider ? provider->get() : nullptr;

            ConfigurableInContext::onConfigureBegin();
        }

        void onConfigureBegin() override
        {
            EXIT_EARLY_IF(!c,);
            c->onConfigureBegin();
        }

        bool doConfigure(QString const& aKey, QString const& aValue) override
        {
            if (!c)
            {
                LOG_WARN(this) << "trying to configure a non-configurable component with " << aKey << "=" << aValue;
                return true;
            }

            if (!c->doConfigure(aKey, aValue))
            {
                THROW(Constants::errorParamRejected().arg(forId, aKey, aValue));
            }

            return true;
        }

        bool onConfigureEnd() override
        {
            EXIT_EARLY_IF(!c, true);
            return c->onConfigureEnd();
        }
    };


    struct ParamList: IpcAdapter::Core::IConfigurator
    {
        QList<QPair<QString, QString>> parameters;

        bool doConfigure(IConfigurable& aConfigurable) override
        {
            auto allParametersAccepted = true;

            for (auto const& parameter : parameters)
            {
                if (!aConfigurable.doConfigure(parameter.first, parameter.second))
                {
                    allParametersAccepted = false;
                }
            }

            return allParametersAccepted;
        }
    };


    struct HandlerContext
    {
        RuntimeConfiguration& configuration;
        HandlerStack handlerStack;

        std::shared_ptr<IpcAdapter::Core::Pipeline> currentPipeline;

        std::shared_ptr<IComponent> currentComponent;
        std::shared_ptr<ConfigurableInContext> currentConfigurable;
        QString currentId;

        std::shared_ptr<ParamList> currentParamList;

        explicit HandlerContext(RuntimeConfiguration& aConfiguration): configuration(aConfiguration) {}

        bool createNewComponent(const QXmlAttributes& atts)
        {
            auto const& type = atts.value("type");
            auto const factory = IpcAdapter::Core::GlobalComponentRegistry::get().getFactoryFor(type);

            if (!factory)
            {
                THROW(Constants::errorUnknownComponent().arg(type));
            }

            currentId = atts.value("id");

            if (configuration.containsComponentCalled(currentId))
            {
                THROW(Constants::errorDuplicateComponentId().arg(type, currentId));
            }

            currentComponent = factory();
            currentConfigurable = std::make_shared<ConfigurableInContext>(currentComponent, currentId);
            return true;
        }

        void storeComponentAndClearContextForNext()
        {
            configuration.addComponent(currentId, currentComponent);

            currentId.clear();
            currentComponent.reset();
            currentConfigurable.reset();
        }

        void storePipelineAndClearContextForNext()
        {
            configuration.addPipeline(currentId, currentPipeline);
            currentId.clear();
            currentPipeline.reset();
        }

        template <class T>
        T* getComponentAs(QString const& aRef, QString const& anXmlElementName)
        {
            auto const component = configuration.getComponent(aRef);
            auto const typedComponent = std::dynamic_pointer_cast<T>(component);

            if (!typedComponent)
            {
                THROW(Constants::errorWrongComponentType().arg(aRef, currentId, anXmlElementName));
            }

            return typedComponent.get();
        }
    };


    struct BaseHandler : QXmlDefaultHandler
    {
        QString ownLocalName;
        HandlerContext& context;
        explicit BaseHandler(HandlerContext& aContext, QString const& anOwnLocalName)
            : ownLocalName(anOwnLocalName), context(aContext) {}

        bool endElement(const QString&, const QString& localName, const QString&) override
        {
            if (localName == ownLocalName)
            {
                context.handlerStack.pop();
            }

            return true;
        }
    };


    struct PipelineHandler: BaseHandler
    {
        explicit PipelineHandler(HandlerContext& aContext) : BaseHandler(aContext, "pipelines") {}

        bool startElement(const QString&, const QString& localName, const QString&, const QXmlAttributes& atts) override
        {
            if (localName == "pipeline")
            {
                context.currentId = atts.value("id");
                context.currentPipeline = std::make_shared<IpcAdapter::Core::Pipeline>();
                return true;
            }

            auto const& ref = atts.value("ref");

            if (localName == "source")
            {
                auto sourceMultiplex = context.configuration.getSourceMultiplexFor(ref);
                sourceMultiplex->sourceTo(context.currentPipeline.get());
            }

            else if (localName == "sink")
            {
                auto const sink = context.getComponentAs<IpcAdapter::Core::ISink>(ref, localName);
                context.currentPipeline->addSink(sink);
            }

            else if (localName == "converter")
            {
                auto const converter = context.getComponentAs<IpcAdapter::Core::IConverter>(ref, localName);
                context.currentPipeline->addConverter(converter);
            }

            return true;
        }

        bool endElement(const QString& nsUri, const QString& localName, const QString& qName) override
        {
            BaseHandler::endElement(nsUri, localName, qName);

            if (localName == "pipeline" && context.currentPipeline)
            {
                context.storePipelineAndClearContextForNext();
            }

            return true;
        }
    };


    struct ComponentHandler : BaseHandler
    {
        explicit ComponentHandler(HandlerContext& aContext) : BaseHandler(aContext, "components") {}

        bool startElement(const QString&, const QString& localName, const QString&, const QXmlAttributes& atts) override
        {
            if (localName == "component")
            {
                return context.createNewComponent(atts);
            }

            if (context.currentComponent)
            {
                if (localName == "param")
                {
                    return context.currentConfigurable->doConfigure(atts.value("key"), atts.value("value"));
                }

                if (localName == "param-list")
                {
                    auto const& paramListId = atts.value("ref");
                    auto const& configurator = context.configuration.getParamList(paramListId);

                    if (!configurator)
                    {
                        THROW(Constants::errorUnknownParamList().arg(context.currentId, paramListId));
                    }

                    return configurator->doConfigure(*context.currentConfigurable.get());
                }
            }

            return true;
        }

        bool endElement(const QString& nsUri, const QString& localName, const QString& qName) override
        {
            BaseHandler::endElement(nsUri, localName, qName);

            if (localName == "component" && context.currentComponent)
            {
                if (!context.currentConfigurable->onConfigureEnd())
                {
                    THROW(Constants::errorFinishingConfiguration().arg(context.currentId));
                }

                context.storeComponentAndClearContextForNext();

                return true;
            }

            return true;
        }
    };


    struct ParamListHandler: BaseHandler
    {
        explicit ParamListHandler(HandlerContext& aContext) : BaseHandler(aContext, "param-lists") {}

        bool startElement(const QString&, const QString& localName, const QString&, const QXmlAttributes& atts) override
        {
            if (localName == "param-list")
            {
                auto const& id = atts.value("id");
                context.currentParamList = std::make_shared<ParamList>();
                context.configuration.addParamList(id, context.currentParamList);
                return true;
            }

            if ((context.currentParamList) && (localName == "param"))
            {
                context.currentParamList->parameters.append({atts.value("key"), atts.value("value")});
            }

            return true;
        }

        bool endElement(const QString& nsUri, const QString& localName, const QString& qName) override
        {
            BaseHandler::endElement(nsUri, localName, qName);

            if (localName == "param-list")
            {
                context.currentParamList.reset();
            }

            return true;
        }
    };


    struct MainSectionsHandler: BaseHandler
    {
        explicit MainSectionsHandler(HandlerContext& aContext) : BaseHandler(aContext, "configuration") {}

        bool startElement(const QString&, const QString& localName, const QString&, const QXmlAttributes&) override
        {
            if (localName == "param-lists")
            {
                context.handlerStack.push(std::make_shared<ParamListHandler>(context));
            }
            else if (localName == "components")
            {
                context.handlerStack.push(std::make_shared<ComponentHandler>(context));
            }
            else if (localName == "pipelines")
            {
                context.handlerStack.push(std::make_shared<PipelineHandler>(context));
            }
            else
            {
                return false;
            }

            return true;
        }
    };


    struct RootElementHandler : BaseHandler
    {
        explicit RootElementHandler(HandlerContext& aContext) : BaseHandler(aContext, "") {}

        bool startElement(const QString&, const QString& localName, const QString&, const QXmlAttributes& atts) override
        {
            if (localName != "configuration")
            {
                THROW(Constants::errorUnsupportedRootElement().arg(localName));
            }

            auto const& version = atts.value("version");

            if (version != "1")
            {
                THROW(Constants::errorWrongRootElementVersion().arg(version));
            }

            context.handlerStack.push(std::make_shared<MainSectionsHandler>(context));
            return true;
        }
    };

}



struct ConfigurationHandler::Data
{
    explicit Data(RuntimeConfiguration& aConfiguration):
        context(aConfiguration)
    {
        context.handlerStack.append(std::make_shared<RootElementHandler>(context));
    }

    bool setErrorStringFrom(QString const& aLevel, const QXmlParseException& anException)
    {
        errorString = QStringLiteral("%1 [%2,%3]: %4")
                      .arg(aLevel)
                      .arg(anException.lineNumber()).arg(anException.columnNumber())
                      .arg(anException.message());
        return false;
    }

    QString errorString;
    HandlerContext context;
};



bool ConfigurationHandler::fatalError(const QXmlParseException& exception)
{
    return d->setErrorStringFrom("fatalError", exception);
}

QString ConfigurationHandler::errorString() const
{
    return d->errorString;
}



bool ConfigurationHandler::startElement
(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts)
{
    if (namespaceURI != Constants::supportedNamespace())
    {
        THROW(Constants::errorUnsupportedNamespace().arg(namespaceURI));
    }

    return d->context.handlerStack.top()->startElement(namespaceURI, localName, qName, atts);
}

bool ConfigurationHandler::endElement(const QString& namespaceURI, const QString& localName, const QString& qName)
{
    return d->context.handlerStack.top()->endElement(namespaceURI, localName, qName);
}



ConfigurationHandler::ConfigurationHandler(RuntimeConfiguration& aConfiguration)
    : d(std::make_unique<Data>(aConfiguration)) {}
ConfigurationHandler::~ConfigurationHandler() = default;
