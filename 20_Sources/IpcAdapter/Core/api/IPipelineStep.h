#pragma once

#include "Shared/api/ConvenienceMacros.h"
#include "Core/api/IPipelineFrame.h"



namespace IpcAdapter
{
    namespace Core
    {
        /// smart pointer carrying a shared abstract pipeline frame
        using PipelineFramePtr = std::shared_ptr<IPipelineFrame>;

        /**
         * An abstraction of a step in a pipeline.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IPipelineStep
        {
        public:
            INTERFACE_DESTRUCTOR(IPipelineStep);

            /**
             * Forwards `aPipelineFrame` into the pipeline step for processing.
             *
             * @param aPipelineFrame  pipeline frame to process in the step
             *
             * @retval true     if the frame was processed successfully
             * @retval false    if the frame failed to be processed
             */
            virtual bool process(PipelineFramePtr aPipelineFrame) = 0;
        };

        /// smart pointer carrying a shared abstract pipeline step
        using PipelineStepPtr = std::shared_ptr<IPipelineStep>;
    }
}
