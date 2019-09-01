#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IPipelineFrame);

        /**
         * An IpcAdapter component specialization that processes a given pipeline frame.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class ISink
            : public IComponent
        {
        public:
            INTERFACE_DESTRUCTOR(ISink);

            /**
             * Processes the passed `@aPipelineFrame` in the sink.
             *
             * @param   aPipelineFrame  pipeline frame to process in the sink
             *
             * @retval true     passed pipeline frame was successfully processed in the sink
             * @retval false    processing the pipeline frame in the sink failed
             */
            virtual bool process(IPipelineFrame& aPipelineFrame) = 0;
        };
    }
}
