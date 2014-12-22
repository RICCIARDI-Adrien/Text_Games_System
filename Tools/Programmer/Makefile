# Makefile for Text Games System PC programmer.
CC = gcc
CCFLAGS = -W -Wall
OBJ = Hex_Parser.o Main.o UART.o
BIN = Programmer

all: $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o $(BIN)
	@# Delete useless files
	@rm -f *.o

clean:
	rm -f *.o $(BIN) $(BIN).exe
	
Hex_Parser.o: Hex_Parser.c
	$(CC) $(CCFLAGS) -c Hex_Parser.c -o Hex_Parser.o

Main.o: Main.c
	$(CC) $(CCFLAGS) -c Main.c -o Main.o

UART.o: UART.c
	$(CC) $(CCFLAGS) -c UART.c -o UART.o