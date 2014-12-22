/** @file Menu_Delete_All_Records.c
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
void MenuDeleteAllRecords(void)
{	
	unsigned char i, String_Temp[RECORD_NAME_LENGTH + 1];
	
	// Prepare screen
	ScreenClear();
	
	// Show title
	InterfaceWriteTitle(STRING_MENU_DELETE_ALL_RECORDS_TITLE);
	
	// Ask user for confirmation
	if (InterfaceAskConfirmation(STRING_MENU_DELETE_ALL_RECORDS_ASK_CONFIRMATION))
	{
		// Delete record without overwriting all data (this is the fastest way, but data still remain)
		*String_Temp = 0;
		for (i = 0; i < RECORD_MAXIMUM_COUNT; i++) RecordWriteName(i, String_Temp);
	
		// Show success message
		ScreenSetColor(SCREEN_COLOR_CODE_TEXT_GREEN);
		ScreenWriteString(STRING_MENU_DELETE_ALL_RECORDS_SUCCESS);
		ScreenSetColor(INTERFACE_COLOR_NORMAL);
	}

	ScreenHideCursor();
	// Wait for the user to press the Escape key
	InterfaceWaitForEscapeKey();
}