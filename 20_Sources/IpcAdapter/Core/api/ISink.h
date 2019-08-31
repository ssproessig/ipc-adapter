#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IPipelineStep);

        class ISink
            : public IComponent
        {
        public:
            INTERFACE_DESTRUCTOR(ISink)

            virtual void forwardTo(IPipelineStep& aPipelineStep) = 0;

        };
    }
}
