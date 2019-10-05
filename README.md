# ipc-adapter

Study for an _inter-process adapter_ in C++/Qt 5.

## Status
[![master Win64 status badge][win64-master-badge]][win64-master-link]
[![develop Win64 status badge][win64-develop-badge]][win64-develop-link]
[![develop Win64 coverage][win64-develop-codecov-badge]][win64-develop-codecov-link]
[![MIT License][license-badge]](LICENSE)

## Key words for use in RFCs to Indicate Requirement Levels
The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD NOT", "RECOMMENDED",  "MAY", and "OPTIONAL" in this document and other documents in this repository are to be interpreted as described in [RFC 2119](https://www.ietf.org/rfc/rfc2119.txt).

## Big Picture / Solution to Realize
![ipc-adapter big picture](00_Documentation/big_picture.png)

Customer has
- a **legacy system** that needs to be connected to 
- a new **ESB solution**.

We assume that we 
- receive data in some format from the ESB and 
- need to _convert_, _merge_ and _filter_ it, before 
- forwarding it to the **legacy system**.


## Documentation
- [Usage](00_Documentation/Usage.md)
- [System Requirements](00_Documentation/System_Requirements.md)
- [Design Decisions](00_Documentation/Design_Decisions.md)
- [Development Process](00_Documentation/Development_Process.md)
- [Coding Rules](00_Documentation/Coding_Rules.md) and [Naming Conventions](00_Documentation/Naming_Conventions.md)
- [Module Requirements Specification](20_Sources/MRS.md)

## Sources

Refer to [20_Sources](20_Sources/).

## License

Refer to [LICENSE](LICENSE)


[license-badge]:                https://img.shields.io/badge/license-MIT-007EC7.svg
[win64-master-badge]:           https://ci.appveyor.com/api/projects/status/github/ssproessig/ipc-adapter?branch=master&svg=true&passingText=master%20Win64%20-%20OK&pendingText=master%20Win64%20-pending&failingText=master%20Win64%20-%20failing
[win64-master-link]:            https://ci.appveyor.com/project/ssproessig/ipc-adapter/branch/master
[win64-develop-badge]:          https://ci.appveyor.com/api/projects/status/github/ssproessig/ipc-adapter?branch=develop&svg=true&passingText=develop%20Win64%20-%20OK&pendingText=develop%20Win64%20-pending&failingText=develop%20Win64%20-%20failing
[win64-develop-link]:           https://ci.appveyor.com/project/ssproessig/ipc-adapter/branch/develop
[win64-develop-codecov-badge]:  https://codecov.io/gh/ssproessig/ipc-adapter/branch/develop/graph/badge.svg
[win64-develop-codecov-link]:   https://codecov.io/gh/ssproessig/ipc-adapter
