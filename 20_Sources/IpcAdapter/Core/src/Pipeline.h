#pragma once

#include "Core/api/IPipeline.h"



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(IConverter);
        FORWARD_DECLARE(ISink);

        /**
         * A concrete implementation of a pipeline.  Every frame received will be forwarded to all sinks
         * in order of their addition.
         *
         * TODO: add converter support
         *
         * @author Soeren Sproessig
         * @version 1
         */
        class Pipeline:
            public IPipeline
        {
            NONCOPYABLE(Pipeline);
            PIMPLED_DATA(d);

        public:
            Pipeline();
            virtual ~Pipeline() override;

            /**
             * Adds a converter to the pipeline.  Converter will be added at the end of the converter list and
             * will be processed after all other converters already on the list, receiving the already converted
             * data from its predecessor.
             *
             * NOTE: the pipeline does not take over the ownership of this raw pointer and will not clean up.
             *
             * @param aConverter  converter to add to the list of sinks
             */
            void addConverter(IConverter* aConverter);

            /**
             * Adds a sink to the pipeline.  Sink will be added at the end of the sink list and will be processed
             * after all other sinks already on the list.
             *
             * NOTE: the pipeline does not take over the ownership of this raw pointer and will not clean up.
             *
             * @param aSink  sink to add to the list of sinks
             */
            void addSink(ISink* aSink);

            /// @name IPipelineStep realization
            /// @{
            /**
             * Passes `aPipelineFrame` into the pipeline for forwarding to all sinks.
             *
             * @param aPipelineFrame    pipeline frame to forward
             *
             * @retval true     if all sinks processed it successfully
             * @retval false    if at least one sink failed processing the frame
             */
            bool process(PipelineFramePtr const& aPipelineFrame) override;
            /// @}
        };
    }
}
