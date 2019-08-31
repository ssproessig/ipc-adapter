#pragma once

#include "Core/api/ISink.h"



namespace IpcAdapter
{
    namespace Components
    {
        namespace Sinks
        {
            class UdpSink
                : public Core::ISink
            {
                NONCOPYABLE(UdpSink);

            public:

                UdpSink();
                virtual ~UdpSink();

                /// @name IComponent realization
                /// @{
                Core::IConfigurable& getConfigurable() override;
                /// @}

                /// @name ISink realization
                /// @{
                bool process(Core::IPipelineFrame& aPipelineFrame) override;
                /// @}

                PIMPLED_DATA(d);
            };
        }
    }
}
