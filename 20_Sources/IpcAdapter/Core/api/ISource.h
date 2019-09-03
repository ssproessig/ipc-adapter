#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"
#include "Core/api/IPipelineStep.h"



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * An IpcAdapter component specialization that sources pipeline frame to a pipeline.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class ISource
            : public IComponent
        {
        public:
            INTERFACE_DESTRUCTOR(ISource);

            /**
             * Configures the source to "source" its pipeline frames to the given `aPipelineStep`.
             *
             * NOTE: the source SHALL NOT take over ownership of `aPipelineStep`.
             *
             * @param aPipelineStep     pipeline step to source to
             */
            virtual void sourceTo(IPipelineStep* aPipelineStep) = 0;
        };
    }
}
