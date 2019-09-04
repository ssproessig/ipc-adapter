#pragma once

#include "Core/api/ISink.h"



namespace IpcAdapter
{
    namespace Components
    {
        namespace Sinks
        {
            /**
             * An IpcAdapter component that forwards data received from the pipeline to an AMQP 0.9.1 exchange.
             *
             * @author Soeren Sproessig
             * @version 1
             */
            class AmqpExchangeSink
                : public Core::ISink
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
                Core::IConfigurable* getConfigurable() override;
                /// @}

                /// @name ISink realization
                /// @{
                /**
                 *
                 *
                 * @param	aPipelineFrame	pipeline frame to extract the data from to send
                 *
                 * @retval	true    if data could be sent to 
                 * @retval  false   if data could not be sent successfully
                 */
                bool process(Core::IPipelineFrame const& aPipelineFrame) override;
                /// @}
            };
        }
    }
}
