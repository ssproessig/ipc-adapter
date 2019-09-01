#pragma once

#include "Shared/api/ConvenienceMacros.h"


namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IRuntimeConfiguration);

        class IRuntime
        {
        public:
            INTERFACE_DESTRUCTOR(IRuntime);

            virtual IRuntimeConfiguration const& getRuntimeConfiguration() const = 0;

            virtual void serveForever() = 0;
        };
    }
}
