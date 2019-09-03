#include "Core/api/SimplePipelineFrame.h"

#include <QByteArray>
#include <QMap>
#include <QString>
#include <QVariant>



using IpcAdapter::Core::IPipelineFrame;
using IpcAdapter::Core::SimplePipelineFrame;



struct SimplePipelineFrame::Data
{
    IPipelineFrame::MetaDataMap metaData;
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
    d->metaData = anotherPipelineFrame->getMetaData();
}

SimplePipelineFrame::~SimplePipelineFrame() = default;



IPipelineFrame::RawData const& SimplePipelineFrame::getData() const
{
    return d->rawData;
}


IPipelineFrame::MetaDataMap const& SimplePipelineFrame::getMetaData() const
{
    return d->metaData;
}



void SimplePipelineFrame::setData(IPipelineFrame::RawData const& aRawDataBuffer)
{
    d->rawData = aRawDataBuffer;
}



void SimplePipelineFrame::updateMetaData(QString const& aKey, QVariant const& aValue)
{
    d->metaData[aKey] = aValue;
}
