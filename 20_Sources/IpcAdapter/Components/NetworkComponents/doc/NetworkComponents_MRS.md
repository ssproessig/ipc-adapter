# IpcAdapter Network Components

## R-IPCA-UDPSNK-001: Provide UdpSink
The system SHALL provide a sink called `UdpSink` that provides means to send UDP datagrams from the data received from the pipeline.

## R-IPCA-UDPSNK-002: Make UdpSink target "host" and "port" configurable
The `UdpSink` SHALL support configuring

- `host`  the host as `QHostAddress` to send to
- `port`  the port as `quint16` to send to


## R-IPCA-UDPSRC-001: Provide UdpSource
The system SHALL provide a source called `UdpSource` that provides means to listen for UDP datagrams and forward them to the pipeline.

## R-IPCA-UDPSRC-002: Make UdpSource listener "host" and "port" configurable
The `UdpSource` SHALL support optionally configuring

- `host`  the host as `QHostAddress` where it SHALL listen at
- `port`  the port as `quint16` where it SHALL listen at

## R-IPCA-UDPSRC-003: UdpSource default configuration
The `UdpSource` SHALL default to the following parameter values

- `host`  SHALL default to `127.0.0.1`
- `port`  SHALL default to `6789`

## R-IPCA-UDPSRC-004: Make UdpSource provide sender host and port as meta-data
The `UdpSource` SHALL provide the

- sender host as `QString` called `UdpSource:host`
- sender port as `quint16` called `UdpSource:port`

of the UDP datagram received as meta-data to the pipeline.
