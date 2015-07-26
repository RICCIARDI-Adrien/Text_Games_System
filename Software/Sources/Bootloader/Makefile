# Makefile for Bootloader
include ../../../Tools/Main_Makefile

AS = gpasm
ASFLAGS = -p 16F876 -w 1

# The SOFTWARE_PATH is mandatory for the main Makefile
SOFTWARE_PATH = ../..
BINARY = Bootloader

all: Bootloader.asm
	$(AS) $(ASFLAGS) Bootloader.asm -o $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex
	@# Delete useless files
	@rm -f $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).cod $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).lst
	
clean:
	rm -f $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex