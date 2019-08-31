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

                PIMPLED_DATA(d);
            };
        }
    }
}
