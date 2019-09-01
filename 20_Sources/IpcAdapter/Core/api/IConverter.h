#pragma once

#include "Shared/api/ConvenienceMacros.h"

#include "Core/api/IComponent.h"
#include "Core/api/IPipelineStep.h"



namespace IpcAdapter
{
    namespace Core
    {
        /**
         * An IpcAdapter component specialization that converts data from the pipeline frame.
         *
         * A converter can be used in many pipelines and shall therefore be stateless.
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class IConverter
            : public IComponent
        {
        public:
            INTERFACE_DESTRUCTOR(IConverter);

            /**
             * Invokes the converter. Converter shall taken `anInput` frame, apply its conversion and return
             * its result as a frame as well.
             *
             * @param anInput   input frame to convert
             *
             * @return  result of the conversion
             */
            virtual PipelineFramePtr convert(PipelineFramePtr const& anInput) = 0;
        };
    }
}
