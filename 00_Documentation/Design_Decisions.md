# Design Decisions

## D-IPCA-001: Realization in C++ / Qt 5 / CMake
C++ SHALL be used as programming language, Qt 5 SHALL be used as realization framework, CMake SHALL be used a project-generator.

### Rationale
- we aim for high speeds and fast conversions, hence use C++
- using Qt 5 we already have many features required to realize the system, as XML readers, TCP/Ip communication, logging, and may target many different platforms
- using CMake we can easily cross-compile, hence use the developer's favorite IDE
- CMake comes with CPack that allows creating RPM packages out-of-the-box


## D-IPCA-002: Realize using configurable pipeline architecture
The system SHALL be realized using a pipeline approach. The pipeline SHALL support the following _abstract components_

- _data sources_, that receive data from the outside world and forward them into the pipeline
- _data converters_, that take raw data as input, converts them to an other format and publishes them back to the pipeline
- _data sinks_, that publish data from the pipeline

The pipeline SHALL be _configurable at_ application _startup time_.

### Rationale
- we can start many ipc-adapter applications in parallel
-- making the overall system robust against failures, as one failure does not influence the other processes
-- favors multiple processes over implementing multi-threading and synchronization problems
- using a component approach we can abstract out _interfaces_ better and realize unit tests in a _test-driven development_ approach
- using a configurable "component pipeline" allows us to configure the pipeline at deployment time, not requiring any recompilation
- using components allows us to extend the system at any time without changing existing components, obeying the _open-closed principle_


## D-IPCA-003: Make the pipeline configurable using XML
The pipeline SHALL be configurable using XML.

### Rationale
- we can provide an XSD as help and documentation for users configuring the pipeline
- we can provide validation and graphical editors using off-the-shelve XML editors


## D-IPCA-004: Use RabbitMQ for Routing and Caching
RabbitMQ SHALL be used as message broker between ESB solution and Legacy Systems.

### Rationale
- RabbitMQ supports durable exchanges and queues that guarantee message delivery
- RabbitMQ supports flexible routing via "topic" exchanges and bound queues
- RabbitMQ scales very good in case we need to scale the overall systems throughput


## D-IPCA-005: Repository / Factory pattern usage
Derived from D-IPCA-003: in order to make the pipeline configurable at startup time:

- the system MUST be able to construct components via factories
- the system MUST be able to look-up those factories from a central registry


## D-IPCA-006: Deploy components statically linked
Although we aim for a _modular_ components approach, the system SHALL be deployed as statically linked binary.

Exception: Qt MAY be dynamically linked.

### Rationale
- deploying a binary that statically links all component registries is easier than invoking a dynamic loader
- we may change to dynamically linking component registries at any later time if we need

