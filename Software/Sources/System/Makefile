# Makefile for Text Games System system
include ../../../Tools/Variables.mk

SOFTWARE_PATH = ../..
PROGRAM_OBJECTS_PATH = $(TEXT_GAMES_SYSTEM_OBJECTS_PATH)/System

# System needs custom flags as it is a library and not an application
LDFLAGS = -lib -t PIC16F876

OBJECTS_DRIVERS = $(PROGRAM_OBJECTS_PATH)/Driver_Flash.obj $(PROGRAM_OBJECTS_PATH)/Driver_I2C.obj $(PROGRAM_OBJECTS_PATH)/Driver_Internal_EEPROM.obj $(PROGRAM_OBJECTS_PATH)/Driver_Random.obj $(PROGRAM_OBJECTS_PATH)/Driver_Timer.obj $(PROGRAM_OBJECTS_PATH)/Driver_UART.obj
OBJECTS_SYSTEM = $(PROGRAM_OBJECTS_PATH)/Interrupts_Handler.obj $(PROGRAM_OBJECTS_PATH)/Keyboard.obj $(PROGRAM_OBJECTS_PATH)/Screen.obj $(PROGRAM_OBJECTS_PATH)/Standard_Library.obj $(PROGRAM_OBJECTS_PATH)/System.obj
OBJECTS = $(OBJECTS_DRIVERS) $(OBJECTS_SYSTEM)

all: $(OBJECTS)
	$(TEXT_GAMES_SYSTEM_LD) $(LDFLAGS) $(OBJECTS) -p $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/System
	
clean:
	rm -f $(PROGRAM_OBJECTS_PATH)/* $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/System.lib

#--------------------------------------------------------------------------------------------------
# Drivers
#--------------------------------------------------------------------------------------------------
$(PROGRAM_OBJECTS_PATH)/Driver_Flash.obj: Driver_Flash.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Flash.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_Flash.c

$(PROGRAM_OBJECTS_PATH)/Driver_I2C.obj: Driver_I2C.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_I2C.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_I2C.c
	
$(PROGRAM_OBJECTS_PATH)/Driver_Internal_EEPROM.obj: Driver_Internal_EEPROM.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Internal_EEPROM.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_Internal_EEPROM.c

$(PROGRAM_OBJECTS_PATH)/Driver_Random.obj: Driver_Random.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Random.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_Random.c
	
$(PROGRAM_OBJECTS_PATH)/Driver_Timer.obj: Driver_Timer.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Timer.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_Timer.c
	
$(PROGRAM_OBJECTS_PATH)/Driver_UART.obj: Driver_UART.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_UART.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Interrupts_Handler.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Driver_UART.c

#--------------------------------------------------------------------------------------------------
# System
#--------------------------------------------------------------------------------------------------	
$(PROGRAM_OBJECTS_PATH)/Interrupts_Handler.obj: Interrupts_Handler.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Interrupts_Handler.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Interrupts_Handler.c

$(PROGRAM_OBJECTS_PATH)/Keyboard.obj: Keyboard.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_UART.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Keyboard.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Keyboard.c

$(PROGRAM_OBJECTS_PATH)/Screen.obj: Screen.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_I2C.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_UART.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Screen.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Screen.c

$(PROGRAM_OBJECTS_PATH)/Standard_Library.obj: Standard_Library.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Standard_Library.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) Standard_Library.c

$(PROGRAM_OBJECTS_PATH)/System.obj: System.c $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Flash.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_I2C.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Internal_EEPROM.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Random.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_Timer.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Driver_UART.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Interrupts_Handler.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Keyboard.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Screen.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/Standard_Library.h $(TEXT_GAMES_SYSTEM_INCLUDES_PATH)/System/System.h
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) System.c
