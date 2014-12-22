/** @file Interface.c
 * @see Interface.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Interface.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void InterfaceWriteTitle(unsigned char *String_Title)
{
	unsigned char Column;
	
	// Compute centered title location
	Column = ((INTERFACE_SCREEN_WIDTH - strlen(String_Title) - 4) >> 1) + 1; // -4 for dashes surrounding title, +1 because VT100 coordinates are 1-based
	
	// Write title
	ScreenSetColor(INTERFACE_COLOR_TITLE);
	ScreenSetCursorLocation(1, Column);
	ScreenWriteString("- ");
	ScreenWriteString(String_Title);
	ScreenWriteString(" -\r\n\r\n");
	
	// Reset color
	ScreenSetColor(INTERFACE_COLOR_NORMAL);
}

void InterfaceWaitForEscapeKey(void)
{
	// Show text on the last screen row
	ScreenSetCursorLocation(INTERFACE_SCREEN_HEIGHT, 1);
	ScreenWriteString(STRING_WAIT_FOR_ESCAPE_KEY);
	
	// Wait for Escape key
	while (KeyboardReadCharacter() != KEYBOARD_KEY_CODE_ESCAPE);
}

void InterfaceWriteErrorString(unsigned char *String)
{
	ScreenSetColor(SCREEN_COLOR_CODE_TEXT_RED);
	ScreenWriteString(String);
	ScreenWriteCharacter('\r');
	ScreenWriteCharacter('\n');
	
	// Restore color
	ScreenSetColor(INTERFACE_COLOR_NORMAL);
}

bool InterfaceAskConfirmation(unsigned char *String_Message)
{
	unsigned char Answer;
	bool Is_Answer_Yes;
	
	ScreenSetColor(INTERFACE_COLOR_CONFIRMATION_QUESTION);
	ScreenWriteString(String_Message);
	ScreenSetColor(INTERFACE_COLOR_NORMAL);
	
	// Wait for the 'yes' or 'no' letter corresponding to compiled language
	while (1)
	{
		Answer = KeyboardReadCharacter();
			
		// Parse answer
		if ((Answer == STRING_CONFIRMATION_LETTER_YES) || (Answer == STRING_CONFIRMATION_LETTER_YES + 32))
		{
			Is_Answer_Yes = true;
			break;
		}
		else if ((Answer == STRING_CONFIRMATION_LETTER_NO) || (Answer == STRING_CONFIRMATION_LETTER_NO + 32))
		{
			Is_Answer_Yes = false;
			break;
		}
	}
	
	// Show answer
	ScreenWriteCharacter(Answer);
	ScreenWriteCharacter('\r');
	ScreenWriteCharacter('\n');
	return Is_Answer_Yes;
}