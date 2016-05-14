# Makefile for Text Games System system
include ../../../Tools/Main_Makefile

SOFTWARE_PATH = ../..
OBJECTS_PATH = $(TEXT_GAMES_SYSTEM_OBJECTS_PATH)/System

# System needs custom flags as it is a library and not an application
LDFLAGS = -lib -t PIC16F876

OBJECTS_DRIVERS = $(OBJECTS_PATH)/Driver_Flash.obj $(OBJECTS_PATH)/Driver_I2C.obj $(OBJECTS_PATH)/Driver_Internal_EEPROM.obj $(OBJECTS_PATH)/Driver_Random.obj $(OBJECTS_PATH)/Driver_Timer.obj $(OBJECTS_PATH)/Driver_UART.obj
OBJECTS_SYSTEM = $(OBJECTS_PATH)/Interrupts_Handler.obj $(OBJECTS_PATH)/Keyboard.obj $(OBJECTS_PATH)/Screen.obj $(OBJECTS_PATH)/Standard_Library.obj $(OBJECTS_PATH)/System.obj
OBJECTS = $(OBJECTS_DRIVERS) $(OBJECTS_SYSTEM)

all: $(OBJECTS)
	$(TEXT_GAMES_SYSTEM_LD) $(LDFLAGS) $(OBJECTS) -p $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/System
	
clean:
	rm -f $(OBJECTS_PATH)/* $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/System.lib

#--------------------------------------------------------------------------------------------------
# Drivers
#--------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/Driver_Flash.obj: Driver_Flash.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Flash.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_Flash.c

$(OBJECTS_PATH)/Driver_I2C.obj: Driver_I2C.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_I2C.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_I2C.c
	
$(OBJECTS_PATH)/Driver_Internal_EEPROM.obj: Driver_Internal_EEPROM.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Internal_EEPROM.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_Internal_EEPROM.c

$(OBJECTS_PATH)/Driver_Random.obj: Driver_Random.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Random.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_Random.c
	
$(OBJECTS_PATH)/Driver_Timer.obj: Driver_Timer.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Timer.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_Timer.c
	
$(OBJECTS_PATH)/Driver_UART.obj: Driver_UART.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_UART.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Interrupts_Handler.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_UART.c

#--------------------------------------------------------------------------------------------------
# System
#--------------------------------------------------------------------------------------------------	
$(OBJECTS_PATH)/Interrupts_Handler.obj: Interrupts_Handler.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Interrupts_Handler.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Interrupts_Handler.c

$(OBJECTS_PATH)/Keyboard.obj: Keyboard.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_UART.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Keyboard.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Keyboard.c

$(OBJECTS_PATH)/Screen.obj: Screen.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_I2C.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_UART.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Screen.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Screen.c

$(OBJECTS_PATH)/Standard_Library.obj: Standard_Library.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Standard_Library.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Standard_Library.c

$(OBJECTS_PATH)/System.obj: System.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Flash.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_I2C.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Internal_EEPROM.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Random.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Timer.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_UART.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Interrupts_Handler.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Keyboard.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Screen.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Standard_Library.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/System.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) System.c
