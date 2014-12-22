# Makefile for EEPROM strings converter.
CC = gcc
CCFLAGS = -Wall
OBJ = Main.o Parser.o
BIN = EEPROMStringsConverter.exe

all: $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o $(BIN)
	@# Delete useless files
	@rm -f *.o

clean:
	rm -f *.o $(BIN)
	
Main.o: Main.c
	$(CC) $(CCFLAGS) -c Main.c -o Main.o
	
Parser.o: Parser.c Parser.h
	$(CC) $(CCFLAGS) -c Parser.c -o Parser.o
