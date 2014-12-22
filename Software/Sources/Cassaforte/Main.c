/** @file Main.c
 * Cassaforte is an utility to store strings on an EEPROM with a 200-year guaranteed retention time.
 * @author Adrien RICCIARDI
 * @version 1.0 : 31/08/2013
 * @version 1.1 : 15/09/2014, added a menu to delete all the existing records.
 */
#include <System/System.h>
#include "Menus.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned char Menu_Index;
	
	// Initialize hardware
	SystemInitialize();
	SystemLedOn();
	
	while (1)
	{
		Menu_Index = MenuMain();
		// Go to selected menu
		switch (Menu_Index)
		{
			case 1:
				MenuListRecords();
				break;
			case 2:
				MenuDisplayRecordContent();
				break;
			case 3:
				MenuNewRecord();
				break;
			case 4:
				MenuRenameRecord();
				break;
			case 5:
				MenuDeleteRecord();
				break;
			case 6:
				MenuDeleteAllRecords();
				break;
			default:
				break;
		}

	}
}