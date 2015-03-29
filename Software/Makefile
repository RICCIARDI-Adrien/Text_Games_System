# Top Makefile used to compile all the Text Games System software.
include ../Tools/Main_Makefile

SOFTWARE_PATH = .

define DisplayTitle
	@echo -e "\033[$2m"
	@echo "+-------------------------------------------"
	@echo "| $1"
	@echo "+-------------------------------------------"
	@echo -e "\033[37m"
endef

all:
	$(call DisplayTitle, "Compiling needed tools", 33)
	@cd ../Tools/EEPROMStringsConverter && $(MAKE)
	@cd ../Tools/Programmer && $(MAKE)
	$(call DisplayTitle, "Compiling System", 33)
	@cd Sources/System && $(MAKE)
	$(call DisplayTitle, "Compiling Calculator", 33)
	@cd Sources/Calculator && $(MAKE)
	$(call DisplayTitle, "Compiling Cassaforte", 33)
	@cd Sources/Cassaforte && $(MAKE)
	$(call DisplayTitle, "Compiling Cellular Automaton", 33)
	@cd Sources/Cellular_Automaton && $(MAKE)
	$(call DisplayTitle, "Compiling ePIC Adventures", 33)
	@cd Sources/ePIC_Adventures && $(MAKE)
	$(call DisplayTitle, "Compiling Games", 33)
	@cd Sources/Games && $(MAKE)
	$(call DisplayTitle, "Compiling Tests", 33)
	@cd Sources/Tests && $(MAKE)
	$(call DisplayTitle, "Compiling Timestamp Storage", 33)
	@cd Sources/Timestamp_Storage && $(MAKE)
	$(call DisplayTitle, "Build successful", 32)
	
clean:
	@rm -f $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/*
	@cd Sources/System && $(MAKE) clean
	@cd Sources/Calculator && $(MAKE) clean
	@cd Sources/Cassaforte && $(MAKE) clean
	@cd Sources/Cellular_Automaton && $(MAKE) clean
	@cd Sources/ePIC_Adventures && $(MAKE) clean
	@cd Sources/Games && $(MAKE) clean
	@cd Sources/Tests && $(MAKE) clean
	@cd Sources/Timestamp_Storage && $(MAKE) clean