#pragma once

#include "Shared/api/ConvenienceMacros.h"



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IPipelineStep);

        class ISink
        {
        public:
            INTERFACE_DESTRUCTOR(ISink)

            virtual void forwardTo(IPipelineStep& aPipelineStep) = 0;

        };
    }
}
