# Makefile for Cellular Automaton
include ../../../Tools/Main_Makefile

# The SOFTWARE_PATH and OBJECTS_PATH are mandatory for the main Makefile
SOFTWARE_PATH = ../..
OBJECTS_PATH = $(TEXT_GAMES_SYSTEM_OBJECTS_PATH)/Cellular_Automaton
OBJECTS = $(OBJECTS_PATH)/Cell.obj $(OBJECTS_PATH)/Main.obj $(OBJECTS_PATH)/World.obj
BINARY = Cellular_Automaton

all: $(OBJECTS)
	$(TEXT_GAMES_SYSTEM_LD) $(TEXT_GAMES_SYSTEM_LDFLAGS) $(OBJECTS) -d $(OBJECTS_PATH) -p $(BINARY)
	@cp $(OBJECTS_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_BINARIES_PATH) > /dev/null
	
$(OBJECTS_PATH)/Cell.obj: Cell.c Cell.h Configuration.h 
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Cell.c
	
$(OBJECTS_PATH)/Main.obj: Main.c Cell.h Configuration.h World.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Main.c
	
$(OBJECTS_PATH)/World.obj: World.c Cell.h Configuration.h World.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) World.c

clean:
	rm -f $(OBJECTS_PATH)/* $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex
	
download:
	$(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Programmer/Programmer.exe $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Empty_EEPROM_Image.dat $(TEXT_GAMES_SYSTEM_PROGRAMMER_UART_DEVICE_NAME)
