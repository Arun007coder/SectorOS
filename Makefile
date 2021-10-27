GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
DEBUG = -DDEBUG=true
ASPARAMS = --32
LDPARAMS = -melf_i386
SHELL = /bin/bash

objects = LILO/loader.o kernel/gdt.o Drivers/IOPorts.o CPU/Interrupts.o Drivers/Driver.o CPU/PowerControl.o Drivers/Keyboard.o Drivers/Mouse.o Drivers/RTC.o Hardcom/SerialPort.o CPU/interruptstab.o Hardcom/pci.o kernel/kernel.o
DEBUGOBJ = LILO/loader.o kernel/gdt.o Drivers/IOPorts.o CPU/Interrupts.o Drivers/Driver.o CPU/PowerControl.o Drivers/Keyboard.o Drivers/Mouse.o Drivers/RTC.o CPU/interruptstab.o Hardcom/pci.o kernel/kernel.o

prep:
	sudo apt-get install xorriso mtools
	mkdir ~/local
	tar xf DEP/grub.tar.xz -C ~/local/

%.o: %.cpp
	@echo "[1/3] Compiling $<"
	gcc $(DEBUG) $(GCCPARAMS) -c -o $@ $<

%.o: %.s
	@echo "[1/3] Compiling $<"
	as $(ASPARAMS) -o $@ $<

SectorOS.bin: LILO/linker.ld prep $(objects)
	@echo "[2/3] Linking object files"
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: SectorOS.bin
	sudo cp $< /boot/SectorOS.bin

SectorOS.iso: SectorOS.bin
	@echo "[3/3] Building ISO file"
	cp SectorOS.bin Build_files/
	mkdir -pv iso
	mkdir -pv iso/boot
	mkdir -pv iso/boot/grub/
	cp $< iso/boot
	echo 'set timeout=3' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "SectorOS" { '>> iso/boot/grub/grub.cfg
	echo 'multiboot /boot/SectorOS.bin' >> iso/boot/grub/grub.cfg
	echo 'boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	~/local/bin/grub-mkrescue --output=$@ iso
	rm -rf iso

move: SectorOS.iso
	mv *.iso SectorOS.bin Build_files/

runQEMU: SectorOS.iso
	sudo qemu-system-i386 -boot d -cdrom SectorOS.iso -chardev serial,path=/dev/ttyS0,id=hostusbserial -device pci-serial,chardev=hostusbserial -soundhw pcspk

runVBOX: SectorOS.iso
	(killall VirtualBoxVM && sleep 1) || true
	VirtualBoxVM --startvm 'SectorOS' --dbg

stopVBOX:
	killall VirtualBoxVM

.PHONY: clean
clean: move
	rm -f $(objects) SectorOS.bin SectorOS.iso

clear:
	@echo "Cleaning the build files" 
	rm -f $(objects) SectorOS.bin SectorOS.iso

SectOS_DBG.iso:
