#include "AmqpExchangeSink.h"

#include "Core/api/IConfigurable.h"
#include "Core/api/IPipelineFrame.h"
#include "Core/api/GlobalComponentRegistry.h"
#include "Core/api/Logger.h"



using IpcAdapter::Components::Sinks::AmqpExchangeSink;
using IpcAdapter::Core::IPipelineFrame;



struct AmqpExchangeSink::Data {};



IpcAdapter::Core::IConfigurable* AmqpExchangeSink::getConfigurable()
{
    return nullptr;
}



bool AmqpExchangeSink::process(IPipelineFrame const& aPipelineFrame)
{
    return false;
}



AmqpExchangeSink::AmqpExchangeSink(): d(std::make_unique<Data>()) {}
AmqpExchangeSink::~AmqpExchangeSink() = default;



REGISTER_COMPONENT(AmqpExchangeSink)
