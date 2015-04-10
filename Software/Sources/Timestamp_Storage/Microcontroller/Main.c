/** @file Main.c
 * Store a timestamp sent from the UART in the internal EEPROM.
 * Two slots are used to avoid data corruption if a power loss occurs during a timestamp writing.
 * The slot number is rotated on each new timestamp write.
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/12/2014
 */
#include <System/System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** A timestamp size in bytes. */
#define TIMESTAMP_SIZE_BYTES 5

// The command values are arbitrary numbers which are difficult to be accidentally issued on the serial port
/** This command writes a timestamp to the EEPROM on the next slot. */
#define COMMAND_WRITE_NEXT_TIMESTAMP 0x4B
/** This command reads the two timestamp slots. */
#define COMMAND_READ_TIMESTAMPS 0x97

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned char Byte, i, Current_Slot = 0, Timestamp[TIMESTAMP_SIZE_BYTES], Address;
	
	// Initialize the system
	SystemInitialize();
	SystemLedOn();

	while (1)
	{
		// Wait for a command
		Byte = UARTReadByte();
		switch (Byte)
		{
			case COMMAND_WRITE_NEXT_TIMESTAMP:
				// Receive the timestamp
				for (i = 0; i < TIMESTAMP_SIZE_BYTES; i++) Timestamp[i] = UARTReadByte();
				
				// Compute the slot base address in the EEPROM (avoiding a multiplication) and the next slot number
				if (Current_Slot == 0)
				{
					Address = 0;
					Current_Slot = 1;
				}
				else
				{
					Address = TIMESTAMP_SIZE_BYTES;
					Current_Slot = 0;
				}
				
				// Write the timestamp data to the selected slot
				for (i = 0; i < TIMESTAMP_SIZE_BYTES; i++) InternalEEPROMWriteByte(Address + i, Timestamp[i]);
				break;
				
			case COMMAND_READ_TIMESTAMPS:
				// Send the two slots content (the two slots are stored contiguously in memory)
				for (i = 0; i < TIMESTAMP_SIZE_BYTES * 2; i++)
				{
					Byte = InternalEEPROMReadByte(i);
					UARTWriteByte(Byte);
				}
				break;
			
			// Do nothing on unknown command
			default:
				break;
		}
	}
}