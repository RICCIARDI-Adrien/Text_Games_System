/** @file Main.c
 * Simple tool to reboot the board in bootloader mode (useful when the programmer can't physically access to the board).
 * @author Adrien RICCIARDI
 * @version 1.0 : 03/11/2013
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "UART.h"

// Code to reboot the running board
#define CODE_REBOOT 0xFE

int main(int argc, char *argv[])
{
	// Check parameters
	if (argc != 2)
	{
		printf("Usage : %s UARTDeviceFileName\n", argv[0]);
		return -1;
	}
	
	// Initialize the UART device
	printf("Configuring computer serial port... ");
	fflush(stdout);
	if (!UARTOpen(argv[1]))
	{
		printf("Error : can't initialize UART (%s).\n", strerror(errno));
		return -2;
	}
	printf("done.\n");
		
	// Send reboot code
	printf("Sending reboot code... ");
	fflush(stdout);
	UARTWriteByte(CODE_REBOOT);
	printf("done.\n");
	
	UARTClose();
	return 0;
}
