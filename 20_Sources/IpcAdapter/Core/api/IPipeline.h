#pragma once

#include "Core/api/IPipelineStep.h"



namespace IpcAdapter
{
    namespace Core
    {
        /**
         *
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
