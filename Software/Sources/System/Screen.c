/** @file Screen.c
 * @see Screen.h
 * @author Adrien RICCIARDI
 */
#include <Driver_I2C.h>
#include <Driver_UART.h>
#include <Screen.h>
#include <Standard_Library.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenWriteString(unsigned char *String)
{
	while (*String != 0)
	{
		UARTWriteByte(*String);
		String++;
	}
}

void ScreenWriteCharacter(unsigned char Character)
{
	UARTWriteByte(Character);
}

void ScreenWriteUnsignedInteger(unsigned int Integer)
{
	unsigned char String_Number[6];
	
	itoa(Integer, String_Number);
	ScreenWriteString(String_Number);
}

void ScreenWriteROMString(unsigned short Address)
{
	unsigned char Character;
	
	// Set EEPROM internal pointer on the beginning of the string
	I2CSetCurrentAddress(Address);
	
	while (1)
	{
		Character = I2CReadNextByte();
		if (Character == 0) return;
		UARTWriteByte(Character);
	}
}

void ScreenClear(void)
{
	// Clear screen
	ScreenWriteString("\x1B[2J");
	// Move cursor to left upper corner
	ScreenWriteString("\x1B[H");
}

void ScreenSetCursorLocation(unsigned char Row, unsigned char Column)
{
	unsigned char String_Number[3];
	
	// Use VT100 code <ESC>[{ROW};{COLUMN}f
	ScreenWriteCharacter('\x1B');
	ScreenWriteCharacter('[');
	itoa(Row, String_Number);
	ScreenWriteString(String_Number);
	ScreenWriteCharacter(';');
	itoa(Column, String_Number);
	ScreenWriteString(String_Number);
	ScreenWriteCharacter('f');
}

// It's lighter to create a procedure-like function than to give multiple times the string into the program
// This is due to the code overheat needed to give a string parameter to a function each time it's called
void ScreenHideCursor(void)
{
	ScreenWriteString("\x1B[?25l");
}

void ScreenShowCursor(void)
{
	ScreenWriteString("\x1B[?25h");
}

void ScreenMoveCursorDown(void)
{
	ScreenWriteCharacter('\n');
	ScreenWriteCharacter('\b');
}

void ScreenDisableScrolling(void)
{
	ScreenWriteString("\x1B[?6l");
}

void ScreenEnableScrolling(void)
{
	ScreenWriteString("\x1B[?6h");
}

void ScreenSetColor(unsigned char Color_Code)
{
	unsigned char String_Number[3];
	
	// Use VT100 code <ESC>[{ATTRIBUTE}m
	ScreenWriteCharacter('\x1B');
	ScreenWriteCharacter('[');
	itoa(Color_Code, String_Number);
	ScreenWriteString(String_Number);
	ScreenWriteCharacter('m');
}