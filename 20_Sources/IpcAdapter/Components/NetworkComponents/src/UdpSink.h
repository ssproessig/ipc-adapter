#pragma once

#include "Core/api/IProvider.h"
#include "Core/api/ISink.h"



namespace IpcAdapter
{
    FORWARD_DECLARE_NS(Core, IConfigurable)

    namespace Components
    {
        namespace NetworkComponents
        {
            /**
             * An IpcAdapter component that forwards data received from the pipeline to a configured UDP target.
             *
             * @author Soeren Sproessig
             * @version 1
             */
            class UdpSink
                : public Core::ISink
                , public Core::IProvider<Core::IConfigurable>
            {
                NONCOPYABLE(UdpSink);
                PIMPLED_DATA(d);

            public:
                UdpSink();
                ~UdpSink() override;

                /// @name IProvider&lt;IConfigurable&gt; realization
                /// @{
                /**
                 * Configures the UdpSink.
                 *
                 * Mandatory parameters:
                 *
                 * - `host` to send the UDP data to
                 * - `port` to send the UDP data to
                 */
                Core::IConfigurable* get() const override;
                /// @}

                /// @name ISink realization
                /// @{
                /**
                 * Sends the data from the `aPipelineFrame` to the configured `host` and `port`.
                 *
                 * @param	aPipelineFrame	pipeline frame to extract the data from to send
                 *
                 * @retval	true    if data could be sent to `host` and `port` configured
                 * @retval  false   if data could not be sent successfully
                 */
                bool process(Core::IPipelineFrame const& aPipelineFrame) override;
                /// @}
            };
        }
    }
}
