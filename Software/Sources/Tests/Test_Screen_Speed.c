/** @file Test_Screen_Speed.c
 * @see Test_Screen_Speed.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Test_Screen_Speed.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void TestScreenSpeed(void)
{
	unsigned char Letter = 'A', i, Row, Column;
	
	for (i = 0; i < 26; i++)
	{
		for (Row = 0; Row < TEST_SCREEN_SPEED_SCREEN_ROWS_COUNT; Row++)
		{
			for (Column = 0; Column < TEST_SCREEN_SPEED_SCREEN_COLUMNS_COUNT; Column++) ScreenWriteCharacter(Letter);
		}
		Letter++;
	}	
}