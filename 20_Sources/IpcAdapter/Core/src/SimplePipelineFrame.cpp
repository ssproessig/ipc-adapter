#include "Core/api/SimplePipelineFrame.h"

#include <QByteArray>



using IpcAdapter::Core::SimplePipelineFrame;



struct SimplePipelineFrame::Data
{
    QByteArray rawData;
};



SimplePipelineFrame::SimplePipelineFrame()
    : d(std::make_unique<Data>())
{
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
