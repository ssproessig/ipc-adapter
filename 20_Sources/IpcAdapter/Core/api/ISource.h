#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"
#include "Core/api/IPipelineStep.h"



namespace IpcAdapter
{
    namespace Core
    {
        class ISource
            : public IComponent
        {
        public:
            INTERFACE_DESTRUCTOR(ISource);

            virtual void sourceTo(PipelineStepPtr const& aPipelineStep) = 0;
        };
    }
}
