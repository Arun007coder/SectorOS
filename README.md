# SectorOS

A Simple program which runs from the boot sector

Proper shutdown is implemented. To call shutdown, press `escape` key on keyboard

## Building

To build the project, First Run:

```shell
./build -I
```

To install required tools. Then run:

```shell
./build -b
```

to build the project. The output will be in the Build folder.

## Known Issues

* Reading Storage medium does not return a value in Floppy disk image
* Keyboard input will work but the background colour is always black in a real machine and in vmware workstation
  