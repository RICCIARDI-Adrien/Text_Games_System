# Makefile for Calculator
include ../../../Tools/Main_Makefile

# The SOFTWARE_PATH and OBJECTS_PATH are mandatory for the main Makefile
SOFTWARE_PATH = ../..
OBJECTS_PATH = $(TEXT_GAMES_SYSTEM_OBJECTS_PATH)/Calculator
OBJECTS = $(OBJECTS_PATH)/Interface.obj $(OBJECTS_PATH)/Main.obj $(OBJECTS_PATH)/Math.obj
BINARY = Calculator

all: $(OBJECTS)
	$(TEXT_GAMES_SYSTEM_LD) $(TEXT_GAMES_SYSTEM_LDFLAGS) $(OBJECTS) -d $(OBJECTS_PATH) -p $(BINARY)
	@cp $(OBJECTS_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_BINARIES_PATH) > /dev/null
	
$(OBJECTS_PATH)/Interface.obj: Interface.c Interface.h Math.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Interface.c
	
$(OBJECTS_PATH)/Main.obj: Main.c Math.h Strings.h Interface.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Main.c
	
$(OBJECTS_PATH)/Math.obj: Math.c Math.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Math.c
	
clean:
	rm -f $(OBJECTS_PATH)/* $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex
	
download:
	$(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Programmer/Programmer.exe $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Empty_EEPROM_Image.bin $(TEXT_GAMES_SYSTEM_PROGRAMMER_UART_DEVICE_NAME)
