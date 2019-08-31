#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IPipelineFrame);

        class ISink
            : public IComponent
        {
        public:
            INTERFACE_DESTRUCTOR(ISink)

            virtual bool process(IPipelineFrame& aPipelineFrame) = 0;

        };
    }
}
