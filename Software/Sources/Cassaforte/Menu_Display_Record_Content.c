/** @file Menu_Display_Record_Content.c
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
void MenuDisplayRecordContent(void)
{
	unsigned char String_Temp[RECORD_TOTAL_LENGTH + 1], String_Record_Name[RECORD_NAME_LENGTH + 1], Record_Number;
	
	// Prepare screen
	ScreenHideCursor();
	ScreenClear();
	
	// Show title
	InterfaceWriteTitle(STRING_MENU_DISPLAY_RECORD_CONTENT_TITLE);
	
	// Ask user for record number
	ScreenWriteString(STRING_MENU_DISPLAY_RECORD_CONTENT_ASK_USER_FOR_RECORD_NUMBER);
	ScreenShowCursor();
	if (!KeyboardReadString(String_Temp, 2, true)) return; // Exit menu if the user pressed Escape
	ScreenHideCursor();
	
	// Check record number validity
	Record_Number = atoi(String_Temp);
	if ((Record_Number < 1) || (Record_Number > RECORD_MAXIMUM_COUNT))
	{
		InterfaceWriteErrorString(STRING_ERROR_BAD_RECORD_NUMBER);
		goto End;
	}
	Record_Number--; // Record numbers start from 0
	
	// Check if the record is empty or not
	RecordReadName(Record_Number, String_Record_Name);
	if (*String_Record_Name == 0)
	{
		InterfaceWriteErrorString(STRING_ERROR_EMPTY_RECORD);
		goto End;
	}
	
	// Show record name
	ScreenWriteString(STRING_MENU_DISPLAY_RECORD_CONTENT_RECORD_NAME_1);
	ScreenWriteString(String_Record_Name);
	ScreenWriteString(STRING_MENU_DISPLAY_RECORD_CONTENT_RECORD_NAME_2);
	
	// Show record content
	RecordReadContent(Record_Number, String_Temp);
	ScreenWriteString(String_Temp);
	
End:
	// Wait for the user to press the Escape key
	InterfaceWaitForEscapeKey();
}