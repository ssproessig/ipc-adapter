#pragma once

#include "Core/api/IProvider.h"
#include "Core/api/ISink.h"

FORWARD_DECLARE(QAmqpClient);
using AmqpClientPtr = std::shared_ptr<QAmqpClient>;



namespace IpcAdapter
{
    FORWARD_DECLARE_NS(Core, IConfigurable)

    namespace Components
    {
        namespace AmqpComponents
        {
            /**
             * An IpcAdapter component that forwards data received from the pipeline to an AMQP 0.9.1 exchange.
             *
             * @author Soeren Sproessig
             * @version 1
             */
            class AmqpExchangeSink
                : public Core::ISink
                , public Core::IProvider<Core::IConfigurable>
            {
                NONCOPYABLE(AmqpExchangeSink);
                PIMPLED_DATA(d);

            public:
                AmqpExchangeSink();
                ~AmqpExchangeSink() override;

                /// @name IComponent realization
                /// @{
                /**
                 * Configures the AmqpExchangeSink.
                 *
                 * Mandatory parameters:
                 *
                 */
                Core::IConfigurable* get() const override;
                /// @}

                /// @name ISink realization
                /// @{
                /**
                 *
                 *
                 * @param	aPipelineFrame	pipeline frame to extract the data from to send
                 *
                 * @retval	true    if data could be sent to AMQP exchange
                 * @retval  false   if data could not be sent successfully
                 */
                bool process(Core::IPipelineFrame const& aPipelineFrame) override;
                /// @}


                /**
                 * Overwrites the QAmqpClient used internally.
                 *
                 * Used to spy on the QAmqpClient API invocation.
                 *
                 * @param anAmqpClient  a spying AMQP client to use
                 */
                void setAmqpClient(AmqpClientPtr const& anAmqpClient);
            };
        }
    }
}
