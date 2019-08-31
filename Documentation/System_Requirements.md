# Functional System Requirements

## R-IPCA-001: Support TIBCO EMS
The system MUST provide means to receive raw data from TIBCO EMS.

## R-IPCA-002: Support Legacy Systems
The system MUST provide means to send raw data to connected legacy systems.

## R-IPCA-003: Convert Data
The system MUST provide means to convert raw data received into other formats before sending to legacy systems.

## R-IPCA-004: Support Routing
The system MUST provide means to route the received and converted data to one of the connected legacy systems based on information encoded in the data received.

## R-IPCA-005: Support Caching and Persistence
The system MUST provide means to cache data received and processed if a legacy system is not reachable. The system MUST NOT lose information received before forwarding to the legacy systems.


# Non-Functional System Requirements

## R-IPCA-010: Support Logging
The system MUST provide means to persistently log its operation steps.

Rationale: it shall be possible to understand why the system behaved the way it behaved in retrospect.

## R-IPCA-011: Support Execution Environment
The system MUST run in the execution environment provided by the customer.

The system MUST support _Red Hat Enterprise Linux 7.3_ and MUST provide RPM installation packages for deployment.

## R-IPCA-012: Performance Indications
The system 

- MUST support receiving upto 2000 messages per seconds
- incoming message sizes SHALL be upto 1024 bytes large.
