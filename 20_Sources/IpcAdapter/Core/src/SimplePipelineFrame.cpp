#include "Core/api/SimplePipelineFrame.h"

#include <QByteArray>



using IpcAdapter::Core::SimplePipelineFrame;



struct SimplePipelineFrame::Data
{
    QByteArray rawData;
};



SimplePipelineFrame::SimplePipelineFrame(): d(std::make_unique<Data>()) {}

SimplePipelineFrame::SimplePipelineFrame(QByteArray const& anInitialDataBuffer): d(std::make_unique<Data>())
{
    d->rawData = anInitialDataBuffer;
}

SimplePipelineFrame::SimplePipelineFrame
(std::shared_ptr<IPipelineFrame> const& anotherPipelineFrame): d(std::make_unique<Data>())
{
    d->rawData = anotherPipelineFrame->getData();
}

SimplePipelineFrame::~SimplePipelineFrame() = default;



QByteArray const& SimplePipelineFrame::getData() const
{
    return d->rawData;
}



void SimplePipelineFrame::setData(QByteArray const& aRawDataBuffer)
{
    d->rawData = aRawDataBuffer;
}
