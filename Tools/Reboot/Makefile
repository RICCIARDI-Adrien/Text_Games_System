# Makefile for Text Games System PC reboot utility.
CC = gcc
CCFLAGS = -Wall
OBJ = Main.o UART.o
BIN = Reboot

all: $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o $(BIN)
	@# Delete useless files
	@rm -f *.o
	
clean:
	rm -f *.o $(BIN) $(BIN).exe

Main.o: Main.c
	$(CC) $(CCFLAGS) -c Main.c -o Main.o

UART.o: UART.c
	$(CC) $(CCFLAGS) -c UART.c -o UART.o