#include "Core/src/Pipeline.h"
#include "Core/api/ISink.h"
#include "Core/api/Logger.h"



using IpcAdapter::Core::Pipeline;
using IpcAdapter::Core::PipelineFramePtr;
using IpcAdapter::Core::ISink;



struct Pipeline::Data
{
    bool process(PipelineFramePtr const& aPipelineFrame)
    {
        IPipelineFrame const& frame = *aPipelineFrame.get();

        for (auto& sink : sinks)
        {
            sink->process(frame);
        }

        return false;
    }

    QList<ISink*> sinks;
};



void Pipeline::addSink(ISink* aSink)
{
    d->sinks.append(aSink);
}



bool Pipeline::process(PipelineFramePtr const& aPipelineFrame)
{
    return d->process(aPipelineFrame);
}



Pipeline::Pipeline(): d(std::make_unique<Data>()) {}
Pipeline::~Pipeline() = default;
