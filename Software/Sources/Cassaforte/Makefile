# Makefile for Cassaforte
include ../../../Tools/Main_Makefile

# The SOFTWARE_PATH and OBJECTS_PATH are mandatory for the main Makefile
SOFTWARE_PATH = ../..
OBJECTS_PATH = $(TEXT_GAMES_SYSTEM_OBJECTS_PATH)/Cassaforte
OBJECTS = $(OBJECTS_PATH)/Interface.obj $(OBJECTS_PATH)/Main.obj $(OBJECTS_PATH)/Menu_Delete_All_Records.obj $(OBJECTS_PATH)/Menu_Delete_Record.obj $(OBJECTS_PATH)/Menu_Display_Record_Content.obj $(OBJECTS_PATH)/Menu_List_Records.obj $(OBJECTS_PATH)/Menu_Main.obj $(OBJECTS_PATH)/Menu_New_Record.obj $(OBJECTS_PATH)/Menu_Rename_Record.obj $(OBJECTS_PATH)/Records.obj
BINARY = Cassaforte

all: $(OBJECTS)
	$(TEXT_GAMES_SYSTEM_LD) $(TEXT_GAMES_SYSTEM_LDFLAGS) $(OBJECTS) -d $(OBJECTS_PATH) -p $(BINARY)
	@cp $(OBJECTS_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_BINARIES_PATH) > /dev/null
	
$(OBJECTS_PATH)/Interface.obj: Interface.c Interface.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Interface.c
	
$(OBJECTS_PATH)/Main.obj: Main.c Interface.h Menus.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Main.c
	
$(OBJECTS_PATH)/Menu_Delete_All_Records.obj: Menu_Delete_All_Records.c Menus.h Records.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Menu_Delete_All_Records.c
 
$(OBJECTS_PATH)/Menu_Delete_Record.obj: Menu_Delete_Record.c Menus.h Records.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Menu_Delete_Record.c
	
$(OBJECTS_PATH)/Menu_Display_Record_Content.obj: Menu_Display_Record_Content.c Menus.h Records.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Menu_Display_Record_Content.c
	
$(OBJECTS_PATH)/Menu_List_Records.obj: Menu_List_Records.c Menus.h Records.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Menu_List_Records.c
	
$(OBJECTS_PATH)/Menu_Main.obj: Menu_Main.c Menus.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Menu_Main.c
	
$(OBJECTS_PATH)/Menu_New_Record.obj: Menu_New_Record.c Menus.h Records.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Menu_New_Record.c
	
$(OBJECTS_PATH)/Menu_Rename_Record.obj: Menu_Rename_Record.c Menus.h Records.h Strings.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Menu_Rename_Record.c
	
$(OBJECTS_PATH)/Records.obj: Records.c Records.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Records.c

clean:
	rm -f $(OBJECTS_PATH)/* $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex
	
download:
	$(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Programmer/Programmer.exe $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Empty_EEPROM_Image.dat $(TEXT_GAMES_SYSTEM_PROGRAMMER_UART_DEVICE_NAME)
