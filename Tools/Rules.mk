# Contain all build rules common to all programs

#--------------------------------------------------------------------------------------------------
# Variables
#--------------------------------------------------------------------------------------------------
include $(SOFTWARE_PATH)/../Tools/Variables.mk

# Program-dependent variables
PROGRAM_OBJECTS_PATH = $(TEXT_GAMES_SYSTEM_OBJECTS_PATH)/$(BINARY)
PROGRAM_OBJECTS = $(patsubst %.c,$(PROGRAM_OBJECTS_PATH)/%.obj,$(SOURCES))
# Let the variable empty if no EEPROM image is defined, so all depending rules won't be executed
ifneq ($(EEPROM_IMAGE),)
    PROGRAM_EEPROM_IMAGE = $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(EEPROM_IMAGE)
endif

#--------------------------------------------------------------------------------------------------
# Rules
#--------------------------------------------------------------------------------------------------
all: headers_check eeprom_image $(PROGRAM_OBJECTS)
	@# Link the program
	$(TEXT_GAMES_SYSTEM_LD) $(TEXT_GAMES_SYSTEM_LDFLAGS) $(PROGRAM_OBJECTS) -d $(PROGRAM_OBJECTS_PATH) -p $(BINARY)
	@# Copy the binary in the right location
	@cp $(PROGRAM_OBJECTS_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_BINARIES_PATH) > /dev/null

# Force the whole program to be recompiled when a header file is modified
$(PROGRAM_OBJECTS_PATH)/headers: *.h
	@echo "Following header files have changed : $?, rebuilding all."
	$(MAKE) clean
	touch $(PROGRAM_OBJECTS_PATH)/headers

# Call the headers rule only if the sources contain one or more headers, or the *.h prerequisite will fail
headers_check:
    ifneq ($(shell find . -name "*.h"),)
		$(MAKE) $(PROGRAM_OBJECTS_PATH)/headers
    endif

# This rule will be called only if the EEPROM image must be generated
$(PROGRAM_EEPROM_IMAGE): Strings.txt
	$(TEXT_GAMES_SYSTEM_EEPROM_STRINGS_CONVERTER) Strings.txt Strings.h $(PROGRAM_EEPROM_IMAGE) $(TEXT_GAMES_SYSTEM_EEPROM_SIZE)

# This rule allows two thing : build the EEPROM image only for the program that need it, and when needed build the EEPROM image only when the Strings.txt file has changed
eeprom_image:
    ifneq ($(EEPROM_IMAGE),)
		$(MAKE) $(PROGRAM_EEPROM_IMAGE)
    endif

# Generic compilation rule compiling each .c file to an .obj file (all sources are rebuilded whenever a header file changes to avoid using .h dependencies)
$(PROGRAM_OBJECTS_PATH)/%.obj: %.c 
	$(TEXT_GAMES_SYSTEM_CC) $(TEXT_GAMES_SYSTEM_CCFLAGS) $<

download:
    ifeq ($(EEPROM_IMAGE),)
		$(TEXT_GAMES_SYSTEM_PROGRAMMER) $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(TEXT_GAMES_SYSTEM_TOOLS_PATH)/Empty_EEPROM_Image.dat $(TEXT_GAMES_SYSTEM_PROGRAMMER_UART_DEVICE_NAME)
    else
		$(TEXT_GAMES_SYSTEM_PROGRAMMER) $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(PROGRAM_EEPROM_IMAGE) $(TEXT_GAMES_SYSTEM_PROGRAMMER_UART_DEVICE_NAME)
    endif

clean:
	rm -f $(PROGRAM_OBJECTS_PATH)/* $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/$(BINARY).hex $(PROGRAM_EEPROM_IMAGE)
