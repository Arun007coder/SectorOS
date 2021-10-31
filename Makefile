GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
DEBUG = -DDEBUG=true
ASPARAMS = --32
LDPARAMS = -melf_i386
SHELL = /bin/bash
grubbin = ~/local/bin

objects = LILO/loader.o \
kernel/gdt.o \
Drivers/IOPorts.o \
CPU/Interrupts.o \
kernel/MultiTask.o \
Drivers/Driver.o \
CPU/PowerControl.o \
Drivers/Keyboard.o \
Drivers/Mouse.o \
Drivers/RTC.o \
Hardcom/SerialPort.o \
CPU/interruptstab.o \
Hardcom/pci.o \
kernel/kernel.o

prep:
	@printf "Preparing...\n"
	@sudo apt-get install xorriso mtools grub-common

%.o: %.cpp
	@printf "\e[1;32m[1/3]Compiling $<\n\e[0m"
	gcc $(DEBUG) $(GCCPARAMS) -c -o $@ $<

%.o: %.s
	@printf "\e[1;32m[1/3]Compiling $<\n\e[0m"
	as $(ASPARAMS) -o $@ $<

SectorOS_Kernel.bin: LILO/linker.ld $(objects)
	@printf "\e[1;33m[2/3]Linking object files\n\e[0m"
	@ld $(LDPARAMS) -T $< -o $@ $(objects)
	@printf "Linking $(objects) to make $@\n"
	@printf "Linking finished\n"

install: SectorOS_Kernel.bin
	sudo cp $< /boot/SectorOS_Kernel.bin

SectorOS.iso: SectorOS_Kernel.bin
	@printf "\e[1;34m[3/3]Building ISO file $@\n\e[0m"
	@mkdir -pv iso
	@mkdir -pv iso/boot
	@mkdir -pv iso/boot/grub/
	@cp $< iso/boot
	@echo 'set timeout=3' >> iso/boot/grub/grub.cfg
	@echo 'set default=0' >> iso/boot/grub/grub.cfg
	@echo '' >> iso/boot/grub/grub.cfg
	@echo 'menuentry "SectorOS" { '>> iso/boot/grub/grub.cfg
	@echo 'multiboot /boot/SectorOS_Kernel.bin' >> iso/boot/grub/grub.cfg
	@echo 'boot' >> iso/boot/grub/grub.cfg
	@echo '}' >> iso/boot/grub/grub.cfg
	@if [ $(grubbin) == "pt" ];then\
		grub-mkrescue --output=$@ iso;\
	else\
		$(grubbin)/grub-mkrescue --output=$@ iso;\
	fi
	@rm -rf iso
	

move: SectorOS.iso
	mkdir Build_files
	mv *.iso SectorOS_Kernel.bin Build_files/

runQEMU: SectorOS.iso
	sudo qemu-system-i386 -boot d -cdrom SectorOS.iso -chardev serial,path=/dev/ttyS0,id=hostusbserial -device pci-serial,chardev=hostusbserial -soundhw pcspk

runVBOX: SectorOS.iso
	@printf "Starting VirtualBox\n";
	@(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm 'SectorOS' --dbg

stopVBOX:
	killall VirtualBoxVM

.PHONY: clean
clean:
	@printf "\e[1;31mCleaning the object files...\n\e[0m"
	@rm -f $(objects) SectorOS_Kernel.bin SectorOS.iso