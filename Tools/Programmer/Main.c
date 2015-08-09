/** @file Main.c
 * PC programmer tool used to program the Text Games System.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/02/2013
 * @version 1.1 : 25/06/2013, added network programming support.
 * @version 1.2 : 27/06/2013, changed program interface to show progression.
 * @version 1.3 : 01/07/2013, removed network programming support as it was too slow and not reliable.
 * @version 1.4 : 09/08/2015, adapted to the new bootloader.
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
/** The bootloader sends this code when it acknowledges received data. */
#define PROTOCOL_CODE_ACKNOWLEDGE 0x42
/** Start the firmware programming. */
#define PROTOCOL_CODE_START_PROGRAMMING 0x5A
/** Send this value as the high byte of a flash word address to stop the flash programming. */
#define PROTOCOL_CODE_PROGRAMMING_FINISHED 0x80

/** Send this value to the firmware to make it reboot in bootloader mode. */
#define PROTOCOL_REBOOT_BOARD_IN_PROGRAMMING_MODE 0xFE

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The .hex file. */
static FILE *File_Program = NULL;
/** The EEPROM image file. */
static FILE *File_EEPROM = NULL;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Close all opened files when exiting. */
static void ExitCloseFiles(void)
{
	if (File_Program != NULL) fclose(File_Program);
	if (File_EEPROM != NULL) fclose(File_EEPROM);
	UARTClose();
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
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
	
	// Check if the program file exists
	File_Program = fopen(argv[1], "rb");
	if (File_Program == NULL)
	{
		printf("Error : can't open program file '%s' (%s).\n", argv[1], strerror(errno));
		return -2;
	}
	atexit(ExitCloseFiles);
	
	// Check if the EEPROM image file exists
	File_EEPROM = fopen(argv[2], "rb");
	if (File_EEPROM == NULL)
	{
		printf("Error : can't open EEPROM file '%s' (%s).\n", argv[2], strerror(errno));
		return -3;
	}
	
	// Initialize the UART device
	if (UARTOpen(argv[3], 19200) != 0)
	{
		printf("Error : can't initialize UART (%s).\n", strerror(errno));
		return -4;
	}

	// Restart the board in bootloader mode
	printf("-> Rebooting the Text Games System in programming mode... ");
	fflush(stdout);
	
	// Send the reboot code (it is ignored by the bootloader if it is in programming mode yet)
	UARTWriteByte(0xFE);
	
	// Send the start programming code
	UARTWriteByte(PROTOCOL_CODE_START_PROGRAMMING);
	
	// Wait for the answer
	Byte = UARTReadByte();
	if (Byte != PROTOCOL_CODE_ACKNOWLEDGE)
	{
		printf("Error : the board sent a bad answer (0x%02X).\n", Byte);
		return -5;
	}
	printf("done.\n");
	
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
					Byte = UARTReadByte();
					if (Byte != PROTOCOL_CODE_ACKNOWLEDGE)
					{
						printf("\nError : the board sent a bad answer (0x%02X).\n", Byte);
						return -5;
					}
					
					// Show status
					Data_Sent_Count++;
					printf("-> Sending program... (instruction %d)\r", Data_Sent_Count);
					fflush(stdout);
				}
				// Signal end of programming to board
				else if (Instructions[i].Is_End_Of_File)
				{
					UARTWriteByte(PROTOCOL_CODE_PROGRAMMING_FINISHED);
					printf("-> Sending program... Program successfully sent (%d instructions).\n", Data_Sent_Count);
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
		// Signal that there is a byte to program by sending something different from PROTOCOL_CODE_PROGRAMMING_FINISHED
		UARTWriteByte(PROTOCOL_CODE_ACKNOWLEDGE);
		
		// Send byte
		UARTWriteByte(Byte);
			
		// Wait for acknowledge
		Byte = UARTReadByte();
		if (Byte != PROTOCOL_CODE_ACKNOWLEDGE)
		{
			printf("\nError : the board sent a bad answer (0x%02X).\n", Byte);
			return -5;
		}
		
		// Show status
		Data_Sent_Count++;
		printf("-> Sending EEPROM data... (byte %d)\r", Data_Sent_Count);
		fflush(stdout);
	}
	
	// Signal end of programming to board
	UARTWriteByte(PROTOCOL_CODE_PROGRAMMING_FINISHED);
	printf("-> Sending EEPROM data... EEPROM successfully programmed (%d bytes).\n", Data_Sent_Count);

	printf("End of programming.\n");
	return 0;
}
