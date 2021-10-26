# SectorOS

A Simple operating system

## Building

To build the project, First Run:

```shell
make SectorOS.iso
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

~~You can get prebuilt iso and kernel from `Build_files` folder~~

The iso image in the Build_files will not boot because the grub-mkrescue is making a uefi bootable disk.

To get unstable builds. go to <https://github.com/Arun007coder/SectorOS/actions>
