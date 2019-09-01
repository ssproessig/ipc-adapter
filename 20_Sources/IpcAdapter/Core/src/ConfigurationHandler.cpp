#include "ConfigurationHandler.h"

#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/IComponent.h"
#include "Core/api/IConfigurable.h"
#include "Core/api/Logger.h"
#include "Core/src/RuntimeConfiguration.h"

#include <QMap>
#include <QStack>
#include <QString>
#include <QXmlParseException>



using IpcAdapter::Core::ConfigurationHandler;
using IpcAdapter::Core::IComponent;
using IpcAdapter::Core::IConfigurable;
using IpcAdapter::Core::RuntimeConfiguration;
using HandlerStack = QStack<QXmlDefaultHandler*>;



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
    }


    struct HandlerContext
    {
        RuntimeConfiguration& configuration;
        HandlerStack handlerStack;

        std::shared_ptr<IComponent> currentComponent;
        QString currentId;
        IConfigurable* currentConfigurable = nullptr;

        explicit HandlerContext(RuntimeConfiguration& aConfiguration): configuration(aConfiguration) {}

        bool createNewComponent(const QXmlAttributes& atts)
        {
            auto const& type = atts.value("type");
            auto const factory = IpcAdapter::Core::GlobalComponentRegistry::get().getFactoryFor(type);

            if (!factory)
            {
                throw std::runtime_error(qPrintable(Constants::errorUnknownComponent().arg(type)));
            }

            currentId = atts.value("id");

            if (configuration.containsComponentCalled(currentId))
            {
                throw std::runtime_error(qPrintable(Constants::errorDuplicateComponentId().arg(type, currentId)));
            }

            currentComponent = factory();
            onConfigureBegin();
            return true;
        }

        void onConfigureBegin()
        {
            currentConfigurable = currentComponent->getConfigurable();

            EXIT_EARLY_IF(!currentConfigurable,);
            currentConfigurable->onConfigureBegin();
        }

        bool doConfigure(QString const& aKey, QString const& aValue)
        {
            if (!currentConfigurable)
            {
                LOG_WARN(this) << "trying to configure a non-configurable component with " << aKey << "=" << aValue;
                return true;
            }

            if (!currentConfigurable->doConfigure(aKey, aValue))
            {
                throw std::runtime_error(
                    qPrintable(Constants::errorParamRejected().arg(currentId, aKey, aValue)));
            }

            return true;
        }

        bool onConfigureEnd()
        {
            EXIT_EARLY_IF(!currentConfigurable, true);
            return currentConfigurable->onConfigureEnd();
        }

        void storeComponentAndClearContextForNext()
        {
            configuration.addComponent(currentId, currentComponent);

            currentId.clear();
            currentComponent.reset();
            currentConfigurable = nullptr;
        }
    };


    struct BaseHandler : QXmlDefaultHandler
    {
        HandlerContext& context;
        explicit BaseHandler(HandlerContext& aContext) : context(aContext) {}
    };


    struct PipelineHandler: BaseHandler
    {
        explicit PipelineHandler(HandlerContext& aContext) : BaseHandler(aContext) {}

        bool startElement(const QString&, const QString& localName, const QString&, const QXmlAttributes& atts) override
        {
            return true;
        }
    };


    struct ComponentHandler : BaseHandler
    {
        explicit ComponentHandler(HandlerContext& aContext) : BaseHandler(aContext) {}

        bool startElement(const QString&, const QString& localName, const QString&, const QXmlAttributes& atts) override
        {
            if (localName == "component")
            {
                return context.createNewComponent(atts);
            }

            if (localName == "param" && context.currentComponent)
            {
                return context.doConfigure(atts.value("key"), atts.value("value"));
            }

            return true;
        }

        bool endElement(const QString&, const QString& localName, const QString&) override
        {
            if (localName == "component" && context.currentComponent)
            {
                if (!context.onConfigureEnd())
                {
                    throw std::runtime_error(
                        qPrintable(Constants::errorFinishingConfiguration().arg(context.currentId)));
                }

                context.storeComponentAndClearContextForNext();

                return true;
            }

            if (localName == "components")
            {
                context.handlerStack.push(new PipelineHandler(context));
            }

            return true;
        }
    };


    struct RootElementHandler : BaseHandler
    {
        explicit RootElementHandler(HandlerContext& aContext) : BaseHandler(aContext) {}

        bool startElement(const QString&, const QString& localName, const QString&, const QXmlAttributes& atts) override
        {
            if (localName != "configuration")
            {
                throw std::runtime_error(qPrintable(Constants::errorUnsupportedRootElement().arg(localName)));
            }

            auto const& version = atts.value("version");

            if (version != "1")
            {
                throw std::runtime_error(qPrintable(Constants::errorWrongRootElementVersion().arg(version)));
            }

            context.handlerStack.push(new ComponentHandler(context));
            return true;
        }
    };

}



struct ConfigurationHandler::Data
{
    explicit Data(RuntimeConfiguration& aConfiguration):
        context(aConfiguration)
    {
        context.handlerStack.append(new RootElementHandler(context));
    }

    ~Data()
    {
        qDeleteAll(context.handlerStack);
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
        throw std::runtime_error(qPrintable(Constants::errorUnsupportedNamespace().arg(namespaceURI)));
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
