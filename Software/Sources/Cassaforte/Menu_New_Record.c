/** @file Menu_New_Record.c
 * @see Menus.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Interface.h"
#include "Menus.h"
#include "Records.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MenuNewRecord(void)
{
	unsigned char String_Temp[RECORD_TOTAL_LENGTH + 1], Record_Number;
	/*rom unsigned char *String_Ask_Record_Number = STRING_MENU_NEW_RECORD_ASK_USER_FOR_RECORD_NUMBER;
	rom unsigned char *String_Ask_Record_Name = STRING_MENU_NEW_RECORD_ASK_RECORD_NAME;
	rom unsigned char *String_Ask_Record_Content = STRING_MENU_NEW_RECORD_ASK_RECORD_CONTENT;
	rom unsigned char *String_Success = STRING_MENU_NEW_RECORD_SUCCESS;*/
		
	// Prepare screen
	ScreenClear();
	
	// Show title
	InterfaceWriteTitle(STRING_MENU_NEW_RECORD_TITLE);
	
	// Ask user for record number
	ScreenWriteString(STRING_MENU_NEW_RECORD_ASK_USER_FOR_RECORD_NUMBER);
	//InterfaceWriteROMString(String_Ask_Record_Number);
	if (!KeyboardReadString(String_Temp, 2, true)) return; // Exit menu if the user pressed Escape
	
	// Check record number validity
	Record_Number = atoi(String_Temp);
	if ((Record_Number < 1) || (Record_Number > RECORD_MAXIMUM_COUNT))
	{
		InterfaceWriteErrorString(STRING_ERROR_BAD_RECORD_NUMBER);
		goto End;
	}
	Record_Number--; // Record numbers start from 0
	
	// Check if the record is empty or not
	RecordReadName(Record_Number, String_Temp);
	if (*String_Temp != 0)
	{
		// Ask the user to overwrite or not the record
		if (!InterfaceAskConfirmation(STRING_MENU_NEW_RECORD_ASK_OVERWRITE_RECORD)) goto End;
	}
	
	// Ask for record name
	ScreenWriteString(STRING_MENU_NEW_RECORD_ASK_RECORD_NAME);
	//InterfaceWriteROMString(String_Ask_Record_Name);
	KeyboardReadString(String_Temp, RECORD_NAME_LENGTH, false);
	// Write record name
	RecordWriteName(Record_Number, String_Temp);
	
	// Ask for record content
	ScreenWriteString(STRING_MENU_NEW_RECORD_ASK_RECORD_CONTENT);
	//InterfaceWriteROMString(String_Ask_Record_Content);
	KeyboardReadString(String_Temp, RECORD_CONTENT_LENGTH, false);
	
	// Write record content
	RecordWriteContent(Record_Number, String_Temp);
	
	// Show success message
	ScreenSetColor(SCREEN_COLOR_CODE_TEXT_GREEN);
	ScreenWriteString(STRING_MENU_NEW_RECORD_SUCCESS);
	//InterfaceWriteROMString(String_Success);
	ScreenSetColor(INTERFACE_COLOR_NORMAL);
	
End:
	ScreenHideCursor();
	// Wait for the user to press the Escape key
	InterfaceWaitForEscapeKey();
}