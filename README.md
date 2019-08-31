# ipc-adapter

Study for an _inter-process adapter_ in C++/Qt 5.

## Big Picture / Solution to Realize
![ipc-adapter big picture](Documentation/big_picture.png)

Customer has
- a **legacy system** that needs to be connected to 
- a new **ESB solution**.

We assume that we 
- receive data in some format from the ESB and 
- need to _convert_, _merge_ and _filter_ it, before 
- forwarding it to the **legacy system**.


## Documentation

- [System Requirements](Documentation/System_Requirements.md)
- [Design Decisions](Documentation/Design_Decisions.md)
- [Coding Rules and Naming Conventions](Documentation/Coding_Rules_and_Naming_Conventions.md)

## Sources

Refer to XYZ.

## License

Refer to [LICENSE](LICENSE)
