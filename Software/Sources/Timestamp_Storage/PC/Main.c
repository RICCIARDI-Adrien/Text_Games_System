/** @file Main.c
 * The Mains Monitor program write the current timestamp each minute into the Text Games System EEPROM.
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/12/2014
 * @version 1.1 : 10/04/2015, replaced old UART drivers with better ones and improved code.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#ifdef WIN32
	#include <windows.h>
#endif
#include "UART.h"

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A timestamp significance. */
typedef struct
{
	unsigned char Day; //! From 1 to 31.
	unsigned char Month; //! From 1 to 12.
	unsigned char Year; //! From 0 to 255 (you must add 2000 to this value).
	unsigned char Hour; //! From 0 to 23.
	unsigned char Minutes; //! From 0 to 59.
} TTimestamp;

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** This command writes a timestamp to the EEPROM on the next slot. */
#define COMMAND_WRITE_NEXT_TIMESTAMP 0x4B
/** This command reads the two timestamp slots. */
#define COMMAND_READ_TIMESTAMPS 0x97

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Read the two last two written timestamps.
 * @param Pointer_Timestamp_1 On output, contain the first slot timestamp.
 * @param Pointer_Timestamp_2 On output, contain the second slot timestamp.
 */
static void ReadAllTimestamps(TTimestamp *Pointer_Timestamp_1, TTimestamp *Pointer_Timestamp_2)
{
	unsigned int i;
	unsigned char *Pointer_Timestamp_Byte;

	// Send the read command
	UARTWriteByte(COMMAND_READ_TIMESTAMPS);

	// Read the first timestamp
	Pointer_Timestamp_Byte = (unsigned char *) Pointer_Timestamp_1;
	for (i = 0; i < sizeof(TTimestamp); i++)
	{
		*Pointer_Timestamp_Byte = UARTReadByte();
		Pointer_Timestamp_Byte++;
	}

	// Read the second timestamp
	Pointer_Timestamp_Byte = (unsigned char *) Pointer_Timestamp_2;
	for (i = 0; i < sizeof(TTimestamp); i++)
	{
		*Pointer_Timestamp_Byte = UARTReadByte();
		Pointer_Timestamp_Byte++;
	}
}

/** Write a timestamp in the next slot.
 * @param Pointer_Timestamp The timestamp to write.
 */
static void WriteTimestamp(TTimestamp *Pointer_Timestamp)
{
	unsigned int i;
	unsigned char *Pointer_Timestamp_Byte;

	// Send the write command
	UARTWriteByte(COMMAND_WRITE_NEXT_TIMESTAMP);

	// Write the timestamp data
	Pointer_Timestamp_Byte= (unsigned char *) Pointer_Timestamp;
	for (i = 0; i < sizeof(TTimestamp); i++)
	{
		UARTWriteByte(*Pointer_Timestamp_Byte);
		Pointer_Timestamp_Byte++;
	}
}

/** Display a timestamp content.
 * @param Pointer_Timestamp The timestamp to display.
 */
static void DisplayTimestamp(TTimestamp *Pointer_Timestamp)
{
	printf("%d/%02d/%04d %02d:%02d\n",
		Pointer_Timestamp->Day,
		Pointer_Timestamp->Month,
		Pointer_Timestamp->Year + 2000,
		Pointer_Timestamp->Hour,
		Pointer_Timestamp->Minutes);
}

/** Automatically close the UART when the program exits. */
static void ExitCloseUART(void)
{
	UARTClose();
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	struct timeval Timeval;
	struct tm *Pointer_Tm;
	char *String_UART_Device, *String_Command;
	TTimestamp Timestamp_1, Timestamp_2;

	// Check parameters
	if (argc != 3)
	{
		printf("Error : bad parameters.\n"
			"Usage : %s UART_Device -m | -s\n"
			"-m : monitor the voltage by writing a timestamp each minute on the TGS.\n"
			"-s : show the two last written timestamps.\n", argv[0]);
		return EXIT_FAILURE;
	}
	String_UART_Device = argv[1];
	String_Command = argv[2];

	// Open the serial port
	if (UARTOpen(String_UART_Device, 19200) != 0)
	{
		printf("Error : could not open the UART device.\n");
		return EXIT_FAILURE;
	}
	atexit(ExitCloseUART);

	// Show the timestamps
	if ((String_Command[0] == '-') && (String_Command[1] == 's'))
	{
		ReadAllTimestamps(&Timestamp_1, &Timestamp_2);

		// Display the timestamps
		DisplayTimestamp(&Timestamp_1);
		DisplayTimestamp(&Timestamp_2);
	}

	// Monitor the timestamp
	else if ((String_Command[0] == '-') && (String_Command[1] == 'm'))
	{
		while (1)
		{
			// Get the current date and time
			if (gettimeofday(&Timeval, NULL) != 0)
			{
				printf("Error : call to gettimeofday() failed.\n");
				return EXIT_FAILURE;
			}

			// Split the time to usable values
			Pointer_Tm = localtime(&Timeval.tv_sec);
			if (Pointer_Tm == NULL)
			{
				printf("Error : call to localtime() failed.\n");
				return EXIT_FAILURE;
			}

			// Fill the timestamp
			Timestamp_1.Day = Pointer_Tm->tm_mday;
			Timestamp_1.Month = Pointer_Tm->tm_mon + 1; // Number of months since january (which is 0)
			Timestamp_1.Year = Pointer_Tm->tm_year - 100; // Number of years since 1900
			Timestamp_1.Hour = Pointer_Tm->tm_hour;
			Timestamp_1.Minutes = Pointer_Tm->tm_min;

			// Write the timestamp to the TGS
			WriteTimestamp(&Timestamp_1);

			// Wait a minute
			#ifdef WIN32
				Sleep(60000);
			#else
				sleep(60);
			#endif
		}
	}
	// Invalid parameter
	else printf("Invalid parameter.\n");

	return EXIT_SUCCESS;
}
