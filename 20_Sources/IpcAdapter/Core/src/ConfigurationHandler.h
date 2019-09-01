#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include <QXmlDefaultHandler>



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(RuntimeConfiguration);

        class ConfigurationHandler:
            public QXmlDefaultHandler
        {
            NONCOPYABLE(ConfigurationHandler);
            PIMPLED_DATA(d);

        public:
            explicit ConfigurationHandler(RuntimeConfiguration& aConfiguration);
            virtual ~ConfigurationHandler() override;


            bool fatalError(const QXmlParseException& exception) override;
            QString errorString() const override;

            bool startElement
            (
                const QString& namespaceURI, const QString& localName,
                const QString& qName, const QXmlAttributes& atts
            ) override;

            bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName) override;

        };

    }
}
