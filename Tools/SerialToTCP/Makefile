CC = gcc
CCFLAGS = -Wall
LDFLAGS = -lbinmode
OBJ = Main.o UART.o
BIN = SerialToTCP

all: $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o $(BIN) $(LDFLAGS)
	@# Clean directory
	@rm -f *.o
	
clean:
	rm -f *.o $(BIN)

Main.o: Main.c UART.h
	$(CC) $(CCFLAGS) -c Main.c -o Main.o

UART.o: UART.c UART.h
	$(CC) $(CCFLAGS) -c UART.c -o UART.o

