/** @file Test_Screen_Speed.h
 * Test how fast the system can write to the screen.
 * @author Adrien RICCIARDI
 * @version 1.0 : 30/04/2013
 */
#ifndef H_TEST_SCREEN_SPEED_H
#define H_TEST_SCREEN_SPEED_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many rows on the screen to test. */
#define TEST_SCREEN_SPEED_SCREEN_ROWS_COUNT 24
/** How many columns on the screen to test. */
#define TEST_SCREEN_SPEED_SCREEN_COLUMNS_COUNT 80

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Test screen characters drawing speed. */
void TestScreenSpeed(void);

#endif