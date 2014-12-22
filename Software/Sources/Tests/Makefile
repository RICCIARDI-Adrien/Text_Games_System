# Makefile for Tests
include ../../../Tools/Main_Makefile

# The SOFTWARE_PATH and OBJECTS_PATH are mandatory for the main Makefile
SOFTWARE_PATH = ../..
OBJECTS_PATH = $(TEXT_GAMES_SYSTEM_OBJECTS_PATH)/Tests
OBJECTS = $(OBJECTS_PATH)/Main.obj $(OBJECTS_PATH)/Test_Clock_Speed.obj $(OBJECTS_PATH)/Test_External_EEPROM.obj $(OBJECTS_PATH)/Test_Random.obj $(OBJECTS_PATH)/Test_Screen_Speed.obj
BINARY = Tests

all: $(OBJECTS)
	$(TEXT_GAMES_SYSTEM_LD) $(TEXT_GAMES_SYSTEM_LDFLAGS) $(OBJECTS) -d $(OBJECTS_PATH) -p $(BINARY)
	@cp $(OBJECTS_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_BINARIES_PATH) > /dev/null
	
$(OBJECTS_PATH)/Main.obj: Main.c Test_Clock_Speed.h Test_External_EEPROM.h Test_Random.h Test_Screen_Speed.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Main.c
	
$(OBJECTS_PATH)/Test_Clock_Speed.obj: Test_Clock_Speed.c Test_Clock_Speed.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Test_Clock_Speed.c
	
$(OBJECTS_PATH)/Test_External_EEPROM.obj: Test_External_EEPROM.c Test_External_EEPROM.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Test_External_EEPROM.c
	
$(OBJECTS_PATH)/Test_Random.obj: Test_Random.c Test_Random.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Test_Random.c
	
$(OBJECTS_PATH)/Test_Screen_Speed.obj: Test_Screen_Speed.c Test_Screen_Speed.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Test_Screen_Speed.c
	
clean:
	rm -f $(OBJECTS_PATH)/* $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex
	
download:
	$(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Programmer/Programmer.exe $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Empty_EEPROM_Image.bin $(TEXT_GAMES_SYSTEM_PROGRAMMER_UART_DEVICE_NAME)
