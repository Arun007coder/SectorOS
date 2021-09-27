# SectorOS

A Simple operating system

## Building

To build the project, First Run:

```shell
make mykernel.iso
```

to build the project. The output will be in the Build folder.  

To run the OS in qemu, Run:

```shell
make run
```

To clean all output files, Run:

```shell
make clean
```

You can get prebuilt iso and kernel from `Build_files` folder

## Bugs

printing capital letters through echo command crashes the os for unknown reason. So shift keys and caps lock are non functional until this bug is removed
