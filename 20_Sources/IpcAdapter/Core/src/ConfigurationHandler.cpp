#include "ConfigurationHandler.h"

#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/IComponent.h"
#include "Core/api/IConfigurable.h"

#include <QMap>
#include <QStack>
#include <QString>
#include <QXmlParseException>



using IpcAdapter::Core::ConfigurationHandler;
using IpcAdapter::Core::IComponent;
using IpcAdapter::Core::IConfigurable;
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
        HandlerStack handlerStack;
        QMap<QString, std::shared_ptr<IComponent>> components;

        std::shared_ptr<IComponent> currentComponent;
        QString currentId;


        bool createNewComponent(const QXmlAttributes& atts)
        {
            auto const& type = atts.value("type");
            auto const factory = IpcAdapter::Core::GlobalComponentRegistry::get().getFactoryFor(type);

            if (!factory)
            {
                throw std::runtime_error(qPrintable(Constants::errorUnknownComponent().arg(type)));
            }

            currentId = atts.value("id");

            if (components.constFind(currentId) != components.constEnd())
            {
                throw std::runtime_error(qPrintable(Constants::errorDuplicateComponentId().arg(type, currentId)));
            }

            currentComponent = factory();
            currentComponent->getConfigurable().onConfigureBegin();
            return true;
        }

        bool configureComponent(QString const& aKey, QString const& aValue)
        {
            if (!currentComponent->getConfigurable().doConfigure(aKey, aValue))
            {
                throw std::runtime_error(
                    qPrintable(Constants::errorParamRejected().arg(currentId, aKey, aValue)));
            }

            return true;
        }

        void storeComponentAndClearContextForNext()
        {
            components.insert(currentId, currentComponent);

            currentId.clear();
            currentComponent.reset();
        }

    };


    struct BaseHandler : QXmlDefaultHandler
    {
        HandlerContext& context;
        explicit BaseHandler(HandlerContext& aContext) : context(aContext) {}
    };


    struct ComponentHandler : BaseHandler
    {
        explicit ComponentHandler(HandlerContext& aContext) : BaseHandler(aContext) {}

        bool startElement(const QString&, const QString& localName, const QString&, const QXmlAttributes& atts) override
        {
            if (localName == "source")
            {
                return context.createNewComponent(atts);
            }

            if (localName == "param" && context.currentComponent)
            {
                return context.configureComponent(atts.value("key"), atts.value("value"));
            }

            return true;
        }

        bool endElement(const QString&, const QString& localName, const QString&) override
        {
            if (localName == "source" && context.currentComponent)
            {
                if (!context.currentComponent->getConfigurable().onConfigureEnd())
                {
                    throw std::runtime_error(
                        qPrintable(Constants::errorFinishingConfiguration().arg(context.currentId)));
                }

                context.storeComponentAndClearContextForNext();
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
    Data()
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



ConfigurationHandler::ConfigurationHandler() : d(std::make_unique<Data>()) {}
ConfigurationHandler::~ConfigurationHandler() = default;
