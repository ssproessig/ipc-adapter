#include "Core/api/SimplePipelineFrame.h"

#include <QByteArray>



using IpcAdapter::Core::IPipelineFrame;
using IpcAdapter::Core::SimplePipelineFrame;



struct SimplePipelineFrame::Data
{
    IPipelineFrame::RawData rawData;
};



SimplePipelineFrame::SimplePipelineFrame(): d(std::make_unique<Data>()) {}

SimplePipelineFrame::SimplePipelineFrame(IPipelineFrame::RawData const& anInitialDataBuffer)
    : d(std::make_unique<Data>())
{
    d->rawData = anInitialDataBuffer;
}

SimplePipelineFrame::SimplePipelineFrame
(std::shared_ptr<IPipelineFrame> const& anotherPipelineFrame)
    : d(std::make_unique<Data>())
{
    d->rawData = anotherPipelineFrame->getData();
}

SimplePipelineFrame::~SimplePipelineFrame() = default;



IPipelineFrame::RawData const& SimplePipelineFrame::getData() const
{
    return d->rawData;
}



void SimplePipelineFrame::setData(IPipelineFrame::RawData const& aRawDataBuffer)
{
    d->rawData = aRawDataBuffer;
}
