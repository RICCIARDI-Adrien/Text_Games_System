AS = gpasm
ASFLAGS = -p 16F876 -w 1

all: Bootloader.asm
	$(AS) $(ASFLAGS) Bootloader.asm -o Bootloader.hex
	@# Delete useless files
	@rm -f *.cod *.lst

clean:
	rm *.hex
