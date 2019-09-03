#include "Core/src/Pipeline.h"
#include "Core/api/IConverter.h"
#include "Core/api/ISink.h"
#include "Core/api/Logger.h"
#include "Core/api/SimplePipelineFrame.h"



using IpcAdapter::Core::Pipeline;
using IpcAdapter::Core::PipelineFramePtr;
using IpcAdapter::Core::IConverter;
using IpcAdapter::Core::ISink;



struct Pipeline::Data
{
    bool process(PipelineFramePtr const& aPipelineFrame)
    {
        bool successfullyProcessed = true;

        PipelineFramePtr frame = std::make_shared<SimplePipelineFrame>(aPipelineFrame->getData());

        for (auto& converter : converters)
        {
            frame = converter->convert(frame);
        }


        IPipelineFrame const& f = *frame.get();

        for (auto& sink : sinks)
        {
            LOG_DEBUG(this) << "processing"  << frame->getData() << "in" << sink;

            if (!sink->process(f))
            {
                successfullyProcessed = false;
            }
        }

        return successfullyProcessed;
    }

    QList<IConverter*> converters;
    QList<ISink*> sinks;
};



void Pipeline::addConverter(IConverter* aConverter)
{
    d->converters.append(aConverter);
}



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
