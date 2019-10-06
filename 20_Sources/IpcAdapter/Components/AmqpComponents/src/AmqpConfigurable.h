#pragma once

#include "Core/api/IConfigurable.h"

#include <functional>



namespace IpcAdapter
{
    namespace Components
    {
        namespace AmqpComponents
        {
            FORWARD_DECLARE_STRUCT(AmqpConfiguration);

            /**
             * An AMQP configuration handler.
             *
             * @author Soeren Sproessig
             * @version 1
             */
            class AmqpConfigurable
                : public Core::IConfigurable
            {
                NONCOPYABLE(AmqpConfigurable);
                NONMOVABLE(AmqpConfigurable);
                PIMPLED_DATA(d);

            public:
                /// a callback to invoke once configuration finished
                using ConfigurationFinishedCallback = std::function<void()>;

                /**
                 * Initializes the `AmqpConfigurable`.
                 *
                 * @param aConfiguration    configuration to store configuration
                 * @param aCallback         callback to invoke after configuration finished successfully
                 */
                AmqpConfigurable(AmqpConfiguration& aConfiguration, ConfigurationFinishedCallback const& aCallback);
                ~AmqpConfigurable() override;

                /// @name IConfigurable implementation
                /// @{
                /**
                 * Begins configuring and initializes the referenced `AmqpConfigurable` to its default values.
                 */
                void onConfigureBegin() override;

                /**
                 * Configures the given parameter `aKey` to `aValue`.
                 *
                 * Connection parameters
                 * - `amqp.host`            AMQP host to connect to
                 * - `amqp.port`            AMQP port to connect to
                 *
                 * Authentication parameters
                 * - `amqp.user`            AMQP user
                 * - `amqp.pwd`             AMQP password
                 * - `amqp.vhost`           AMQP virtual host
                 *
                 * Exchange parameters
                 * - `exchange.name`        AMQP exchange name
                 * - `exchange.type`        AMQP exchange type (supported `direct`, `fanout`, `topic`)
                 *
                 * Publication parameters
                 * - `exchange.routing-key` routing-key to publish messages under
                 *                          use meta-data placeholders `${key}` to replace them with their value
                 *
                 * @param   aKey    parameter to configure
                 * @param   aValue  value to configure the parameter to
                 *
                 * @retval  true    parameter `aKey` was successfully configured to `aValue`
                 * @retval  false   parameter `aKey` failed to configure
                 */
                bool doConfigure(QString const& aKey, QString const& aValue) override;

                /**
                 * Finishes configuring the `AmqpConfigurable`.
                 *
                 * @retval  true    configuration finished successfully
                 * @retval  false   configuration failed
                 */
                bool onConfigureEnd() override;
                /// @}
            };
        }
    }
}
