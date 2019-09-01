#include "ConfigurationHandler.h"

#include <QString>
#include <QXmlParseException>



using IpcAdapter::Core::ConfigurationHandler;



namespace
{
    namespace Constants
    {
        DECLARE_CONST(QString, supportedNamespace, ("urn:ipca-configuration-v1"))
        DECLARE_CONST(QString, errorUnsupportedNamespace, ("configuration has unsupported namespace '%1'!"))
    }
}


struct ConfigurationHandler::Data
{
    bool setErrorStringFrom(QString const& aLevel, const QXmlParseException& anException)
    {
        errorString = QStringLiteral("%1 [%2,%3]: %4")
                      .arg(aLevel)
                      .arg(anException.lineNumber()).arg(anException.columnNumber())
                      .arg(anException.message());
        return false;
    }

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

    return false;
}


ConfigurationHandler::ConfigurationHandler() : d(std::make_unique<Data>()) {}
ConfigurationHandler::~ConfigurationHandler() = default;
