#pragma once

#include "Core/api/IPipelineStep.h"



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * An abstraction to interact with pipelines.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IPipeline
            : public IPipelineStep
        {
        public:
            INTERFACE_DESTRUCTOR(IPipeline);
        };
    }
}
