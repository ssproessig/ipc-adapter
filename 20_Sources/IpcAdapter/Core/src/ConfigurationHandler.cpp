#include "ConfigurationHandler.h"

#include <QXmlParseException>



using IpcAdapter::Core::ConfigurationHandler;


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


ConfigurationHandler::ConfigurationHandler() : d(std::make_unique<Data>()) {}
ConfigurationHandler::~ConfigurationHandler() = default;
