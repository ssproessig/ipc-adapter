#pragma once

#include "Shared/api/ConvenienceMacros.h"


namespace IpcAdapter
{
    namespace Core
    {
        class IRuntime
        {
        public:
            INTERFACE_DESTRUCTOR(IRuntime);

            virtual void configure() = 0;

            virtual void serveForever() = 0;
        };
    }
}
