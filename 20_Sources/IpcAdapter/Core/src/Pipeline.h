#pragma once

#include "Core/api/IPipeline.h"



namespace IpcAdapter
{
    namespace Core
    {
        FORWARD_DECLARE(ISink);

        /**
         *
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

            void addSink(ISink* aSink);

            bool process(PipelineFramePtr const& aPipelineFrame) override;
        };

    }
}
