# Makefile for Mains Monitor.
include ../../../../Tools/Variables.mk

CC = gcc
CCFLAGS = -W -Wall
SOFTWARE_PATH = ../../..
BINARY_NAME = $(TEXT_GAMES_SYSTEM_BINARIES_PATH)/Mains_Monitor

all:
	$(CC) $(CCFLAGS) Main.c UART_Linux.c UART_Windows.c -o $(BINARY_NAME)

clean:
	rm -f $(BINARY_NAME) $(BINARY_NAME).exe
