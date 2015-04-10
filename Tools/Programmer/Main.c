/** @file Main.c
 * PC programmer tool used to program the Text Games System.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/02/2013
 * @version 1.1 : 25/06/2013, added network programming support.
 * @version 1.2 : 27/06/2013, changed program interface to show progression.
 * @version 1.3 : 01:07:2013, removed network programming support as it was too slow and not reliable.
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Hex_Parser.h"
#include "UART.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Code to begin programming. */
#define CODE_PROGRAMMING_START 0xFE
/** Acknowledge code sent by the board. */
#define CODE_ACKNOWLEDGE 0xEF

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	FILE *File_Program, *File_EEPROM;
	unsigned char Byte;
	char String[100];
	TInstruction Instructions[32];
	int Instructions_Count, Data_Sent_Count = 0, i;
	
	// Show title
	printf("+------------------------------------+\n");
	printf("|    Text Games System programmer    |\n");
	printf("+------------------------------------+\n");
	
	// Check parameters
	if (argc != 4)
	{
		printf("Usage : %s Hexadecimal_File EEPROM_Image_File UART_Device_File_Name\n", argv[0]);
		return -1;
	}
	
	// Check if file to transfer exists
	File_Program = fopen(argv[1], "rb");
	if (File_Program == NULL)
	{
		printf("Error : can't open program file '%s' (%s).\n", argv[1], strerror(errno));
		return -2;
	}
	
	// Check if EEPROM image file exists
	File_EEPROM = fopen(argv[2], "rb");
	if (File_EEPROM == NULL)
	{
		printf("Error : can't open EEPROM file '%s' (%s).\n", argv[2], strerror(errno));
		fclose(File_Program);
		return -3;
	}
	
	// Initialize the UART device
	if (!UARTOpen(argv[3]))
	{
		printf("Error : can't initialize UART (%s).\n", strerror(errno));
		fclose(File_Program);
		fclose(File_EEPROM);
		return -4;
	}
		
	// Send code to begin programming
	printf("-> You can turn on the Text Games System now.\n");
	printf("Waiting for connection... ");
	fflush(stdout);
	while (1)
	{
		UARTWriteByte(CODE_PROGRAMMING_START);
		if (UARTIsByteAvailable(&Byte))
		{
			if (Byte == CODE_ACKNOWLEDGE)
			{
				// Send a dummy byte different from CODE_PROGRAMMING_START to put the bootloader in programming mode
				usleep(50000);
				UARTWriteByte('a');
				break;
			}
		}
	}
	printf("Connection to board established.\n");
	
	// Send program instructions
	while (1)
	{
		// Read a line from the hex file
		if (fgets(String, sizeof(String), File_Program) == NULL) break;
		
		// Decode it
		Instructions_Count = HexParserDecodeLine(String, Instructions);
				
		// Send to board
		if (Instructions_Count > 0)
		{
			for (i = 0; i < Instructions_Count; i++)
			{
				if (Instructions[i].Is_Instruction_Valid)
				{
					// Send address high byte (as PIC16F876 has 13-bit wide addresses, we use the remaining bits from the high byte to differentiate bootloader codes from program addresses)
					UARTWriteByte(Instructions[i].Address >> 8);
					// Send address low byte
					UARTWriteByte(Instructions[i].Address);
					// Send high byte
					UARTWriteByte(Instructions[i].Code >> 8);
					// Send low byte
					UARTWriteByte(Instructions[i].Code);
					// Wait for board acknowledge
					while (UARTReadByte() != CODE_ACKNOWLEDGE);
					
					// Show status
					Data_Sent_Count++;
					printf("-> Sending program... (instruction %d)\r", Data_Sent_Count);
					fflush(stdout);
				}
				// Signal end of programming to board
				else if (Instructions[i].Is_End_Of_File)
				{
					UARTWriteByte(CODE_PROGRAMMING_START);
					printf("-> Sending program... Program successfully sent (%d instructions).\n", Data_Sent_Count);
					fclose(File_Program);
					goto Program_EEPROM;
				}
			}
		}
	}
	
Program_EEPROM:
	Data_Sent_Count = 0;
	
	// Read bytes one by one
	while (fread(&Byte, 1, 1, File_EEPROM) == 1)
	{
		// Signal that there is a byte to program by sending something different from CODE_PROGRAMMING_START
		UARTWriteByte(CODE_ACKNOWLEDGE);
		
		// Send byte
		UARTWriteByte(Byte);
			
		// Wait for acknowledge
		while (UARTReadByte() != CODE_ACKNOWLEDGE);
		
		// Show status
		Data_Sent_Count++;
		printf("-> Sending EEPROM data... (byte %d)\r", Data_Sent_Count);
		fflush(stdout);
	}
	
	// Signal end of programming to board
	UARTWriteByte(CODE_PROGRAMMING_START);
	printf("-> Sending EEPROM data... EEPROM successfully programmed (%d bytes).\n", Data_Sent_Count);

	fclose(File_EEPROM);
	UARTClose();
	printf("End of programming.\n");
	return 0;
}
