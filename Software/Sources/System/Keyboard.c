/** @file Keyboard.c
 * @see Keyboard.h for description.
 * @author Adrien RICCIARDI
 */
#include <Driver_UART.h>
#include <Keyboard.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned char KeyboardReadString(unsigned char *String, unsigned char Maximum_Characters_Count, unsigned char Are_Only_Digits_Allowed)
{
	unsigned char i = 0, Character;
	
	while (1)
	{
		Character = UARTReadByte();
		
		switch (Character)
		{
			// Escape
			case KEYBOARD_KEY_CODE_ESCAPE:
				return 0;
				
			// Backspace
			case '\b':
				if (i > 0) // Can't erase an empty string
				{
					// Put a blank space at the location of the last displayed character
					UARTWriteByte('\b');
					UARTWriteByte(' ');
					UARTWriteByte('\b');
					i--;
					String[i] = 0;
				}
				break;
				
			// Enter
			case '\r':
			case '\n':
				if (i > 0) // An empty string is not allowed
				{
					// Go to next line
					UARTWriteByte('\r');
					UARTWriteByte('\n');
					// End string
					String[i] = 0;
					return 1;
				}
				break;
				
			// Other characters
			default:
				if (i < Maximum_Characters_Count) // Can't insert more characters than allowed
				{
					if (Are_Only_Digits_Allowed && ((Character < '0') || (Character > '9'))) break;
					UARTWriteByte(Character);				
					String[i] = Character;
					i++;
				}
				break;
		}
	}
}