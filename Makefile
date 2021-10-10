GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = LILO/loader.o kernel/gdt.o Drivers/IOPorts.o CPU/Interrupts.o Drivers/Driver.o CPU/PowerControl.o Drivers/Keyboard.o Drivers/Mouse.o Drivers/RTC.o Debug/Debug.o CPU/interruptstab.o Hardcom/pci.o kernel/kernel.o

%.o: %.cpp
	gcc $(GCCPARAMS) -c -o $@ $<

%.o: %.s
	as $(ASPARAMS) -o $@ $<

mykernel.bin: LILO/linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

mykernel.iso: mykernel.bin
	cp mykernel.bin Build_files/
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub/
	cp $< iso/boot
	echo 'set timeout=3' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "SectorOS" { '>> iso/boot/grub/grub.cfg
	echo 'multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo 'boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

move: mykernel.iso
	mv *.iso mykernel.bin Build_files/

runQEMU: mykernel.iso
	qemu-system-i386 -boot d -cdrom mykernel.iso -serial file:serial.log

runVBOX: mykernel.iso
	(killall VirtualBoxVM && sleep 1) || true
	VBoxManage startvm 'SectorOS'

stopVBOX:
	killall VirtualBoxVM

.PHONY: clean
clean: move
	rm -f $(objects) mykernel.bin mykernel.iso

clear:
	rm -f $(objects) mykernel.bin mykernel.iso