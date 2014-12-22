AS = gpasm
ASFLAGS = -p 16F876 -w 1

all: Bootloader_Release.asm Bootloader_Development.asm
	$(AS) $(ASFLAGS) -o Bootloader_Release.hex Bootloader_Release.asm
	$(AS) $(ASFLAGS) -o Bootloader_Development.hex Bootloader_Development.asm
	@# Delete useless files
	@rm -f *.cod *.lst

clean:
	rm *.hex
