#pragma once

#include "Core/api/IProvider.h"
#include "Core/api/ISource.h"



namespace IpcAdapter
{
    FORWARD_DECLARE_NS(Core, IConfigurable)

    namespace Components
    {
        namespace NetworkComponents
        {
            /**
             * An IpcAdapter component that forwards data received on an UDP socket to the pipeline.
             *
             * @author Soeren Sproessig
             * @version 1
             */
            class UdpSource
                : public Core::ISource
                , public Core::IProvider<Core::IConfigurable>
            {
                NONCOPYABLE(UdpSource);
                PIMPLED_DATA(d);

            public:
                UdpSource();
                ~UdpSource() override;

                /// @name IProvider&lt;IConfigurable&gt; realization
                /// @{
                /**
                 * Configures the UdpSource.
                 *
                 * Optional parameters:
                 *
                 * - `host` to listen for UDP data at (defaults to `127.0.0.1`)
                 * - `port` to listen for UDP data at (defaults to `6789`)
                 */
                Core::IConfigurable* get() const override;
                /// @}

                /// @name ISource realization
                /// @{
                /**
                 * Configures the source to "source" its pipeline frames to the given `aPipelineStep`.
                 *
                 * @param aPipelineStep     pipeline step to source to
                 */
                void sourceTo(Core::IPipelineStep* aPipelineStep) override;
                /// @}
            };
        }
    }
}
