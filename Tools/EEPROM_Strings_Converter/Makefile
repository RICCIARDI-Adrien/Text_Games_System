# Makefile for EEPROM strings converter.
CC = gcc
CCFLAGS = -W -Wall
BINARY_NAME = EEPROM_Strings_Converter

all:
	$(CC) $(CCFLAGS) Main.c Parser.c -o $(BINARY_NAME)

clean:
	rm -f $(BINARY_NAME) $(BINARY_NAME).exe