# Makefile for Text Games System PC programmer.
CC = gcc
CCFLAGS = -W -Wall -ISerial_Port_Library/Includes
BINARY_NAME = Programmer

all:
	$(CC) $(CCFLAGS) Hex_Parser.c Main.c Serial_Port_Library/Sources/Serial_Port_Linux.c Serial_Port_Library/Sources/Serial_Port_Windows.c -o $(BINARY_NAME)

clean:
	rm -f $(BINARY_NAME) $(BINARY_NAME).exe