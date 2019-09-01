#include "ConfigurationHandler.h"

#include <QStack>
#include <QString>
#include <QXmlParseException>



using IpcAdapter::Core::ConfigurationHandler;
using HandlerStack = QStack<QXmlDefaultHandler*>;



namespace
{
    namespace Constants
    {
        DECLARE_CONST(QString, supportedNamespace, ("urn:ipca-configuration-v1"))
        DECLARE_CONST(QString, errorUnsupportedNamespace, ("configuration has unsupported namespace '%1'!"))
        DECLARE_CONST(QString, errorUnsupportedRootElement, ("configuration has wrong root element '%1'!"))
    }


    struct RootElementHandler: QXmlDefaultHandler
    {
        HandlerStack& stack;
        explicit RootElementHandler(HandlerStack& aStack): stack(aStack) {}

        bool startElement(const QString&, const QString& localName, const QString&, const QXmlAttributes& atts) override
        {
            if (localName != "configuration")
            {
                throw std::runtime_error(qPrintable(Constants::errorUnsupportedRootElement().arg(localName)));
            }

            return true;
        }
    };


}



struct ConfigurationHandler::Data
{
    Data()
    {
        handlerStack.append(new RootElementHandler(handlerStack));
    }

    ~Data()
    {
        qDeleteAll(handlerStack);
    }

    bool setErrorStringFrom(QString const& aLevel, const QXmlParseException& anException)
    {
        errorString = QStringLiteral("%1 [%2,%3]: %4")
                      .arg(aLevel)
                      .arg(anException.lineNumber()).arg(anException.columnNumber())
                      .arg(anException.message());
        return false;
    }

    HandlerStack handlerStack;

    QString errorString;
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

    return d->handlerStack.top()->startElement(namespaceURI, localName, qName, atts);
}


ConfigurationHandler::ConfigurationHandler() : d(std::make_unique<Data>()) {}
ConfigurationHandler::~ConfigurationHandler() = default;
