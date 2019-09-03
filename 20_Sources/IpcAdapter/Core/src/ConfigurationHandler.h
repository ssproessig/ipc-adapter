#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include <QXmlDefaultHandler>



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(RuntimeConfiguration);

        /**
         * An Xml Handler that parses XML files following the schema identified by XML name`ipca-configuration-v1.xsd`.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class ConfigurationHandler:
            public QXmlDefaultHandler
        {
            NONCOPYABLE(ConfigurationHandler);
            PIMPLED_DATA(d);

        public:
            /**
             * Creates the ConfigurationHandler. Fills the passed `aConfiguration` with configuration found in the XML.
             *
             * @param   aConfiguration  Runtime Configuration to fill from this handler
             */
            explicit ConfigurationHandler(RuntimeConfiguration& aConfiguration);
            ~ConfigurationHandler() override;

            /// @name QXmlDefaultHandler error handling specialization
            /// @{
            /**
             * Overwrites the default handler for "fatal errors" to get more insights for returning them with
             * @see errorString.
             *
             * @param exception detailed XML parser exception information
             *
             * @return false    always false to stop processing
             */
            bool fatalError(const QXmlParseException& exception) override;

            /**
             * Returns the detailed error string stored in the handler.
             *
             * @return current error string stored in the handler
             */
            QString errorString() const override;
            /// @}


            /// @name QXmlDefaultHandler element handling code
            /// @{
            /**
             * Handles the start of a new XML element
             *
             * @param namespaceURI      XML namespace of the element
             * @param localName         local name of the element
             * @param qName             qualified element name
             * @param atts              element attributes
             *
             * @retval  true    if the element was successfully parsed and parsing shall continue
             * @retval  false   if the element failed to be parsed and parsing shall be aborted
             */
            bool startElement
            (
                const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts
            ) override;

            /**
             * Handles the end of an XML element
             *
             * @param namespaceURI      XML namespace of the element
             * @param localName         local name of the element
             * @param qName             qualified element name
             *
             * @retval  true    if the element was successfully parsed and parsing shall continue
             * @retval  false   if the element failed to be parsed and parsing shall be aborted
             */
            bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName) override;
            /// @}
        };

    }
}
