#include "ReverseBytesConverter.h"

#include "Core/api/IConfigurable.h"
#include "Core/api/IPipelineStep.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/Logger.h"
#include "Core/api/SimplePipelineFrame.h"

#include <QByteArray>



using IpcAdapter::Components::Converters::ReverseBytesConverter;
using IpcAdapter::Core::PipelineFramePtr;



IpcAdapter::Core::IConfigurable* ReverseBytesConverter::getConfigurable()
{
    return nullptr;
}



PipelineFramePtr ReverseBytesConverter::convert(PipelineFramePtr const& anInput)
{
    auto reversedFrame = std::make_shared<Core::SimplePipelineFrame>(anInput);
    {
        QByteArray reversed = anInput->getData();
        std::reverse(reversed.begin(), reversed.end());
        reversedFrame->setData(reversed);

        LOG_DEBUG(this) << "reversing " << anInput->getData() << "to" << reversed;
    }
    return std::move(reversedFrame);
}



REGISTER_COMPONENT(ReverseBytesConverter)
