GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = LILO/loader.o kernel/gdt.o Drivers/IOPorts.o CPU/Interrupts.o Drivers/Driver.o CPU/PowerControl.o Drivers/Keyboard.o Drivers/Mouse.o Drivers/RTC.o Hardcom/SerialPort.o CPU/interruptstab.o Hardcom/pci.o kernel/kernel.o

%.o: %.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

%.o: %.s
	as $(ASPARAMS) -o $@ $<

SectorOS.bin: LILO/linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: SectorOS.bin
	sudo cp $< /boot/SectorOS.bin

SectorOS.iso: SectorOS.bin
	cp SectorOS.bin Build_files/
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub/
	cp $< iso/boot
	echo 'set timeout=3' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "SectorOS" { '>> iso/boot/grub/grub.cfg
	echo 'multiboot /boot/SectorOS.bin' >> iso/boot/grub/grub.cfg
	echo 'boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
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
	rm -f $(objects) SectorOS.bin SectorOS.iso