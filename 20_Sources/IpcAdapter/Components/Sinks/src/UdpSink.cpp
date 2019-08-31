#include "UdpSink.h"



using IpcAdapter::Components::Sinks::UdpSink;



struct UdpSink::Data {};



UdpSink::UdpSink()
    : d(std::make_unique<Data>())
{}

UdpSink::~UdpSink() = default;
