# IpcAdapter Source Components

## R-IPCA-SOURCE-001: Provide UdpSink
The system SHALL provide a source called `UdpSource` that provides means to listen for UDP datagrams and forward them to the pipeline.

## R-IPCA-SOURCE-002: Make UdpSource listener "host" and "port" configurable
The `UdpSource` SHALL support optionally configuring

- `host`  the host as `QHostAddress` where it SHALL listen at
- `port`  the port as `quint16` where it SHALL listen at

## R-IPCA-SOURCE-003: UdpSource default configuration
The `UdpSource` SHALL default to the following parameter values

- `host`  SHALL default to `127.0.0.1`
- `port`  SHALL default to `6789`

## R-IPCA-SOURCE-004: Make UdpSource provide sender host and port as meta-data
The `UdpSink` SHALL provide the

- sender host as `QString` called `UdpSource:host`
- sender port as `quint16` called `UdpSource:port`

of the UDP datagram received as meta-data to the pipeline.
