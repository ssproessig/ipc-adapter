#pragma once

#include "Shared/api/ConvenienceMacros.h"
#include "Core/api/IPipelineFrame.h"



namespace IpcAdapter
{
    namespace Core
    {
        using PipelineFramePtr = std::shared_ptr<IPipelineFrame>;

        class IPipelineStep
        {
        public:
            INTERFACE_DESTRUCTOR(IPipelineStep);

            virtual bool process(PipelineFramePtr aPipelineFrame) = 0;
        };

        using PipelineStepPtr = std::shared_ptr<IPipelineStep>;
    }
}
