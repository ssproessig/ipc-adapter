# IpcAdapter Sink Components

## R-IPCA-SINK-001: Provide UdpSink
The system SHALL provide a sink called `UdpSink` that provides means to send UDP datagrams from the data received from the pipeline.

## R-IPCA-SINK-002: Make UdpSink target "host" and "port" configurable
The `UdpSink` SHALL support configuring

- `host`  the host as `QHostAddress` to send to
- `port`  the port as `quint16` to send to
