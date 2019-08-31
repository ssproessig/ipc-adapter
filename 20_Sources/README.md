# ipc-adapter Source Code

## Organization

- `CMake` contains `.cmake` files that set-up the project
- `IpcAdapter` contains _our_ sources
   - `Application` contains the sources for the main application
   - `Components` contains the sources for _configurable components_ the application can dynamically load at start-up
   - `Core` contains the sources for the minimum core functionality to setup the ipc-adapter runtime
- `ThirdParty` will later contain _other peoples_ sources that we use as dependencies

## Prerequisites

You need to have

- CMake >= 3.12
- Qt >= 5.9
- git 

on your `PATH`.

## Building the application
1. Create an out-of-source build directory
2. Build with CMake in this directory

```sh
$ # clone repo, e.g. to /home/user/sources/ipc-adapter

$ mkdir /tmp/ipc-adapter-build 
$ cd /tmp/ipc-adapter-build 

$ cmake /home/user/sources/ipc-adapter
$ cmake --build .
```
