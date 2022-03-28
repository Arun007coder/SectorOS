CPP=gcc
AS=as
LD=ld
ASM=nasm
ASMFLAGS= -f elf
CPPFLAGS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASFLAGS = --32
LDFLAGS = -melf_i386
SHELL = /bin/bash
GRUB = ~/local/bin
BOCHS=/usr/local/bin/bochs
DEBUG=-g

objects = LILO/loader.o \
kernel/gdt.o \
Memory/MemoryManagement.o \
Drivers/IOPorts.o \
CPU/Interrupts.o \
CPU/Execptions/GPF.o \
CPU/Execptions/IOC.o \
CPU/Execptions/DFault.o \
Drivers/AM79C973.o \
kernel/MultiTask.o \
Drivers/Driver.o \
CPU/PowerControl.o \
Drivers/Keyboard.o \
Drivers/HDD-ATA.o \
Drivers/VGADriver.o \
Shell/Shell.o \
CPU/syscall.o \
loaders/JD1618.o \
Include/string.o \
Structures/list.o \
Filesystem/MSDOSPT.o \
Filesystem/FATFS.o \
Drivers/Mouse.o \
Drivers/RTC.o \
Hardcom/SerialPort.o \
CPU/interruptstab.o \
Hardcom/pci.o \
Network/EtherFrame.o \
Network/arp.o \
Network/InternetProtocolV4.o \
Network/ICMP.o \
Network/udp.o \
Network/tcp.o \
kernel/kernel.o

prep:
	@printf "Preparing...\n"
	@sudo apt-get install xorriso mtools grub-pc-bin nasm build-essential

CPU/PowerControl.o: CPU/PowerControl.cpp CPU/shutdown.o
	$(CPP) $(CPPFLAGS) -c -o $@ CPU/PowerControl.cpp CPU/shutdown.o

%.o: %.cpp
	@printf "\e[1;32m[1/3]Compiling $<\n\e[0m"
	$(CPP) $(DEBUG) $(CPPFLAGS) -c -o $@ $<

%.o: %.s
	@printf "\e[1;32m[1/3]Compiling $<\n\e[0m"
	$(AS) $(DEBUG) $(ASFLAGS) -o $@ $<

%.o: %.asm
	@printf "\e[1;32m[1/3]Compiling $<\n\e[0m"
	$(ASM) $(ASMFLAGS) -o $@ $<

SectorOS_Kernel.bin: LILO/linker.ld $(objects)
	@printf "\e[1;33m[2/3]Linking object files\n\e[0m"
	@$(LD) $(LDFLAGS) -T $< -o $@ $(objects)
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
	@if [ $(GRUB) == "pt" ];then\
		grub-mkrescue --output=$@ iso;\
	else\
		$(GRUB)/grub-mkrescue --output=$@ iso;\
	fi
	@rm -rf iso
	

move: SectorOS.iso
	@printf "\e[1;35mMoving $< to Build_files\n\e[0m"
	@mkdir Build_files
	@mv *.iso SectorOS_Kernel.bin Build_files/

runQEMU: SectorOS.iso
	qemu-system-i386 -boot d -cdrom SectorOS.iso -s -m 2048  &

runVBOX: SectorOS.iso
	@printf "Starting VirtualBox\n";
	@(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm 'SectorOS' --dbg &

runBOCHS: SectorOS.iso
	@printf "Starting Bochs\n";
	@(killall bochs && sleep 1) || true
	$(BOCHS) -q -f bochsrc.txt &

stopVBOX:
	killall VirtualBoxVM

.PHONY: clean
clean:
	@printf "\e[1;31mCleaning the object files...\n\e[0m"
	@rm -f $(objects) CPU/shutdown.o SectorOS_Kernel.bin SectorOS.iso
