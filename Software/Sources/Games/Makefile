# Makefile for Games
include ../../../Tools/Main_Makefile

# The SOFTWARE_PATH and OBJECTS_PATH are mandatory for the main Makefile
SOFTWARE_PATH = ../..
OBJECTS_PATH = $(TEXT_GAMES_SYSTEM_OBJECTS_PATH)/Games
OBJECTS = $(OBJECTS_PATH)/Brain_Calculation.obj $(OBJECTS_PATH)/Demo.obj $(OBJECTS_PATH)/Loto.obj $(OBJECTS_PATH)/Main.obj $(OBJECTS_PATH)/Numbers.obj $(OBJECTS_PATH)/Shared_Buffer.obj $(OBJECTS_PATH)/Wumpus.obj
BINARY = Games
EEPROM_IMAGE = $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/Games_EEPROM_Image.bin

all: Strings.h $(EEPROM_IMAGE) $(OBJECTS)
	$(TEXT_GAMES_SYSTEM_LD) $(TEXT_GAMES_SYSTEM_LDFLAGS) $(OBJECTS) -d $(OBJECTS_PATH) -p $(BINARY)
	@cp $(OBJECTS_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_BINARIES_PATH) > /dev/null
	
Strings.h: Strings.txt
	$(TEXT_GAMES_SYSTEM_EEPROM_STRINGS_CONVERTER) Strings.txt Strings.h $(EEPROM_IMAGE) $(TEXT_GAMES_SYSTEM_EEPROM_SIZE)
	
$(EEPROM_IMAGE): Strings.txt
	$(TEXT_GAMES_SYSTEM_EEPROM_STRINGS_CONVERTER) Strings.txt Strings.h $(EEPROM_IMAGE) $(TEXT_GAMES_SYSTEM_EEPROM_SIZE)
	
$(OBJECTS_PATH)/Brain_Calculation.obj: Brain_Calculation.c Brain_Calculation.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Brain_Calculation.c
	
$(OBJECTS_PATH)/Demo.obj: Demo.c Demo.h Shared_Buffer.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Demo.c
	
$(OBJECTS_PATH)/Loto.obj: Loto.c Loto.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Loto.c
	
$(OBJECTS_PATH)/Main.obj: Main.c Brain_Calculation.h Loto.h Numbers.h Strings.h Wumpus.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Main.c

$(OBJECTS_PATH)/Numbers.obj: Numbers.c Numbers.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Numbers.c

$(OBJECTS_PATH)/Shared_Buffer.obj: Shared_Buffer.c Shared_Buffer.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Shared_Buffer.c
	
$(OBJECTS_PATH)/Wumpus.obj: Wumpus.c Wumpus.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Wumpus.c

clean:
	rm -f $(OBJECTS_PATH)/* $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(EEPROM_IMAGE)
	
download:
	$(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Programmer/Programmer.exe $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(EEPROM_IMAGE) $(TEXT_GAMES_SYSTEM_PROGRAMMER_UART_DEVICE_NAME)
