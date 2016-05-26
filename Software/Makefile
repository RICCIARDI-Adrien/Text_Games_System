# Top Makefile used to compile all the Text Games System software.
include ../Tools/Variables.mk

SOFTWARE_PATH = .

define DisplayTitle
	@echo
	@echo "+-------------------------------------------"
	@echo "| $1"
	@echo "+-------------------------------------------"
endef

all:
	$(call DisplayTitle, "Compiling needed tools")
	@cd ../Tools/EEPROM_Strings_Converter && $(MAKE)
	@cd ../Tools/Programmer && $(MAKE)
	$(call DisplayTitle, "Compiling System")
	@cd Sources/System && $(MAKE)
	$(call DisplayTitle, "Compiling Calculator")
	@cd Sources/Calculator && $(MAKE)
	$(call DisplayTitle, "Compiling Cassaforte")
	@cd Sources/Cassaforte && $(MAKE)
	$(call DisplayTitle, "Compiling Cellular Automaton")
	@cd Sources/Cellular_Automaton && $(MAKE)
	$(call DisplayTitle, "Compiling ePIC Adventures")
	@cd Sources/ePIC_Adventures && $(MAKE)
	$(call DisplayTitle, "Compiling Games")
	@cd Sources/Games && $(MAKE)
	$(call DisplayTitle, "Compiling Tests")
	@cd Sources/Tests && $(MAKE)
	$(call DisplayTitle, "Compiling Timestamp Storage")
	@cd Sources/Timestamp_Storage/Microcontroller && $(MAKE)
	@cd Sources/Timestamp_Storage/PC && $(MAKE)
	$(call DisplayTitle, "Build successful")
	
clean:
	@cd ../Tools/EEPROM_Strings_Converter && $(MAKE) clean
	@cd ../Tools/Programmer && $(MAKE) clean
	@rm -f $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/*
	@cd Sources/System && $(MAKE) clean
	@cd Sources/Calculator && $(MAKE) clean
	@cd Sources/Cassaforte && $(MAKE) clean
	@cd Sources/Cellular_Automaton && $(MAKE) clean
	@cd Sources/ePIC_Adventures && $(MAKE) clean
	@cd Sources/Games && $(MAKE) clean
	@cd Sources/Tests && $(MAKE) clean
	@cd Sources/Timestamp_Storage/Microcontroller && $(MAKE) clean
	@cd Sources/Timestamp_Storage/PC && $(MAKE) clean