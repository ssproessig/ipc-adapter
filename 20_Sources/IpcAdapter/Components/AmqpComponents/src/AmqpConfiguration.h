#pragma once

#include <QHostAddress>
#include <QString>



namespace IpcAdapter
{
    namespace Components
    {
        namespace AmqpComponents
        {
            /**
             * AMQP configuration "data transfer object"
             *
             * @author Soeren Sproessig
             * @version 1
             */
            struct AmqpConfiguration
            {
                /// @name AMQP connection parameters
                /// @{
                /// AMQP host to connect to
                QHostAddress host;
                /// AMQP port to connect to
                quint16 port = 0;
                /// @}

                /// @name AMQP authentication parameters
                /// @{
                /// AMQP user to authenticate with
                QString user;
                /// AMQP password to authenticate with
                QString pwd;
                /// AMQP virtual host to authenticate at
                QString vhost;
                /// @}

                /// @name AMQP exchange parameters
                /// @{
                /// AMQP exchange to use
                QString exchangeName;
                /// AMQP exchange type to use
                QString exchangeType;
                /// @}

                /// @name AMQP publication parameters
                /// @{
                /// AMQP routing-key to publish messages under
                QString routingKey;
                /// @}

                /// flag that carries whether the `AmqpConfigurable` configured the `AmqpConfiguration` successfully
                bool isOk = false;


                /**
                 * Creates the `AmqpConfiguration` and initializes it to the required default values.
                 */
                AmqpConfiguration();

                /**
                 * Initializes the `AmqpConfiguration` to its defined default values.
                 */
                void initDefaults();


                /// Uri style to return AMQP URI in
                enum class UriStyle : bool
                {
                    /// returns the URI ready for usage
                    ForUsage,
                    /// returns the URI with password replaced with `***`
                    LogSafe
                };

                /**
                 * Returns the AMQP URI from the `AmqpConfiguration`.
                 *
                 * @param anUriStyle    style to return the AMQP URI in
                 *
                 * @return AMQP URI from the configuration
                 */
                QString getAmqpUri(UriStyle const anUriStyle) const;
            };
        }
    }
}
