# Makefile for ePIC Adventures
include ../../../Tools/Main_Makefile

# The SOFTWARE_PATH and OBJECTS_PATH are mandatory for the main Makefile
SOFTWARE_PATH = ../..
OBJECTS_PATH = $(TEXT_GAMES_SYSTEM_OBJECTS_PATH)/ePIC_Adventures
OBJECTS = $(OBJECTS_PATH)/main.obj
BINARY = ePIC_Adventures
EEPROM_IMAGE = $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/ePIC_Adventures_EEPROM_Image.bin

all: Strings.h $(EEPROM_IMAGE) $(OBJECTS)
	$(TEXT_GAMES_SYSTEM_LD) $(TEXT_GAMES_SYSTEM_LDFLAGS) $(OBJECTS) -d $(OBJECTS_PATH) -p $(BINARY)
	@cp $(OBJECTS_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_BINARIES_PATH) > /dev/null
	
Strings.h: help/Strings.txt
	$(TEXT_GAMES_SYSTEM_EEPROM_STRINGS_CONVERTER) help/Strings.txt Strings.h $(EEPROM_IMAGE) $(TEXT_GAMES_SYSTEM_EEPROM_SIZE)
	
$(EEPROM_IMAGE): help/Strings.txt
	$(TEXT_GAMES_SYSTEM_EEPROM_STRINGS_CONVERTER) help/Strings.txt Strings.h $(EEPROM_IMAGE) $(TEXT_GAMES_SYSTEM_EEPROM_SIZE)
	
$(OBJECTS_PATH)/main.obj: main.c characters.h gui.h help/Strings.txt interactions.h items.h tools.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) main.c

clean:
	rm -f $(OBJECTS_PATH)/* $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(EEPROM_IMAGE)
	
download:
	$(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Programmer/Programmer.exe $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(EEPROM_IMAGE) $(TEXT_GAMES_SYSTEM_PROGRAMMER_UART_DEVICE_NAME)
