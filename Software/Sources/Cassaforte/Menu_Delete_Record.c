/** @file Menu_Delete_Record.c
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
void MenuDeleteRecord(void)
{	
	unsigned char Record_Number, String_Temp[RECORD_NAME_LENGTH + 1];
	
	// Prepare screen
	ScreenClear();
	
	// Show title
	InterfaceWriteTitle(STRING_MENU_DELETE_RECORD_TITLE);
	
	// Ask user for record number
	ScreenWriteString(STRING_MENU_DELETE_RECORD_ASK_USER_FOR_RECORD_NUMBER);
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
	if (*String_Temp == 0)
	{
		// Exit as the record is empty
		InterfaceWriteErrorString(STRING_ERROR_EMPTY_RECORD);
		goto End;
	}
	
	// Show current record name
	ScreenWriteString(STRING_MENU_DELETE_RECORD_SHOW_RECORD_NAME_1);
	ScreenWriteString(String_Temp);
	ScreenWriteString(STRING_MENU_DELETE_RECORD_SHOW_RECORD_NAME_2);
	// Ask confirmation
	if (!InterfaceAskConfirmation(STRING_MENU_DELETE_RECORD_ASK_CONFIRMATION)) goto End;
	
	// Delete record without overwriting all data (this is the fastest way, but data still remain)
	*String_Temp = 0;
	RecordWriteName(Record_Number, String_Temp);
	
	// Show success message
	ScreenSetColor(SCREEN_COLOR_CODE_TEXT_GREEN);
	ScreenWriteString(STRING_MENU_DELETE_RECORD_SUCCESS);
	ScreenSetColor(INTERFACE_COLOR_NORMAL);
	
End:
	ScreenHideCursor();
	// Wait for the user to press the Escape key
	InterfaceWaitForEscapeKey();
}