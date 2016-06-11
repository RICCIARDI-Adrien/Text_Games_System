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
all: headers_check $(PROGRAM_EEPROM_IMAGE) $(PROGRAM_OBJECTS)
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
	@# Make the headers file more recent than the newly-generated Strings.h header, or the headers_check rule will trigger a full clean on next call
	touch $(PROGRAM_OBJECTS_PATH)/headers

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
