/** @file Free_Fall.c
 * @see Free_Fall.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Free_Fall.h"
#include "Shared_Buffer.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The player leftmost allowed location. */
#define FREE_FALL_PLAYER_LEFTMOST_COLUMN (((FREE_FALL_SCREEN_COLUMNS_COUNT - FREE_FALL_PIPE_COLUMNS_COUNT) / 2) + 2) // +1 because the VT100 coordinates start from 1, +1 to bypass the pipe leftmost border
/** The player rightmost allowed location. */
#define FREE_FALL_PLAYER_RIGHTMOST_COLUMN (FREE_FALL_PLAYER_LEFTMOST_COLUMN + FREE_FALL_PIPE_COLUMNS_COUNT - 1) // -1 to bypass the pipe rightmost border

/** The player "skin". */
#define FREE_FALL_PLAYER_CHARACTER '#'
/** An obstacle "skin". */
#define FREE_FALL_OBSTACLE_CHARACTER '='

/** How fast the game scrolls. */
#define FREE_FALL_GAME_SPEED_FREQUENCY_DIVIDER_VALUE 32

/** A bit mask that must be the same length than the pipe width. */
#define FREE_FALL_PIPE_WIDTH_BIT_MASK 0x3FFF

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
/** Player current column. */
static unsigned char Free_Fall_Player_Column;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Create the stage set. */
inline void FreeFallInitializeScene(void)
{
	unsigned char Row, i;
	
	ScreenClear();
	
	// Display the pipe
	for (Row = 1; Row < FREE_FALL_SCREEN_ROWS_COUNT; Row++)
	{
		// Display the leftmost border
		ScreenSetCursorLocation(Row, FREE_FALL_PLAYER_LEFTMOST_COLUMN - 1); // One character left from the leftmost allowed player location
		ScreenWriteCharacter('|');
		
		// Fill with spaces until the other border is reached
		for (i = 0; i < FREE_FALL_PIPE_COLUMNS_COUNT; i++) ScreenWriteCharacter(' ');
		// Display the rightmost border
		ScreenWriteString("|\r\n");
	}
	
	// Display the player
	Free_Fall_Player_Column = FREE_FALL_PLAYER_LEFTMOST_COLUMN + (FREE_FALL_PIPE_COLUMNS_COUNT / 2); // Put the player at the pipe's center
	ScreenSetCursorLocation(FREE_FALL_PLAYER_ROW, Free_Fall_Player_Column);
	ScreenWriteCharacter(FREE_FALL_PLAYER_CHARACTER);
	
	// Reset the obstacles array
	memset(Shared_Buffer.Free_Fall_Obstacle_Rows, 0, sizeof(Shared_Buffer.Free_Fall_Obstacle_Rows));
}

/** Generate a row of obstacles each 2 to 5 rows. */
static void FreeFallGenerateObstacles(void) // This function is not inline because SourceBoost does not allow static variables in inline functions
{
	static unsigned char Next_Filled_Row = 0, Empty_Rows_Count = 0; // When to create a new row of obstacles, how many rows were let empty before a new row is generated
	unsigned char Obstacles_Count, i;
	unsigned short Obstacles_Mask = 0;
	
	// Shift obstacles one row up as fast as possible
	memcpy(&Shared_Buffer.Free_Fall_Obstacle_Rows[0], &Shared_Buffer.Free_Fall_Obstacle_Rows[1], sizeof(Shared_Buffer.Free_Fall_Obstacle_Rows) - sizeof(Shared_Buffer.Free_Fall_Obstacle_Rows[0]));
	
	// Do nothing until the row to generate is reached
	if (Empty_Rows_Count < Next_Filled_Row)
	{
		Shared_Buffer.Free_Fall_Obstacle_Rows[FREE_FALL_SCREEN_ROWS_COUNT - 1] = 0; // Clear the row
		Empty_Rows_Count++;
		return;
	}
	
	// Generate the obstacles : get a FREE_FALL_PIPE_COLUMNS_COUNT-bit random number, each set bit stands for an obstacle
	do
	{
		Obstacles_Mask = (RandomGenerateNumber() << 8) | RandomGenerateNumber();
		Obstacles_Mask &= FREE_FALL_PIPE_WIDTH_BIT_MASK; // Avoid using the modulo operator
	} while (Obstacles_Mask == FREE_FALL_PIPE_WIDTH_BIT_MASK); // Avoid a fully-filled row
	// Set the last row value
	Shared_Buffer.Free_Fall_Obstacle_Rows[FREE_FALL_SCREEN_ROWS_COUNT - 1] = Obstacles_Mask;
	
	// Choose when to fill the next row
	Empty_Rows_Count = 0;
	Next_Filled_Row = (RandomGenerateNumber() % 3) + 2; // The next row will be located between 2 to 5 rows from this one
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void FreeFall(void)
{
	unsigned char i, Character, Game_Speed_Frequency_Divider = 0, Is_Player_Crashed = 0, String_Score_Value[6]; // 5 digits + terminating zero
	unsigned short Score = 0;
	
	ScreenHideCursor();
	FreeFallInitializeScene();
	RandomInitialize();
	
	while (1)
	{
		// Shift the obstacles not too fast
		if (Game_Speed_Frequency_Divider < FREE_FALL_GAME_SPEED_FREQUENCY_DIVIDER_VALUE) Game_Speed_Frequency_Divider++;
		else
		{
			// Clear the player trace before scrolling the screen
			ScreenSetCursorLocation(FREE_FALL_PLAYER_ROW, Free_Fall_Player_Column);
			ScreenWriteCharacter(' ');
			
			// Clear the "score" string before scrolling the screen
			ScreenSetCursorLocation(FREE_FALL_SCORE_STRING_ROW, 1);
			for (i = 0; i < 13; i++) ScreenWriteCharacter(' '); // The length of the string "Score : " + 5 digits number
		
			// Generate the next row of obstacles
			FreeFallGenerateObstacles();
			
			// Display it
			ScreenSetCursorLocation(FREE_FALL_SCREEN_ROWS_COUNT, FREE_FALL_PLAYER_LEFTMOST_COLUMN - 1);
			ScreenWriteCharacter('|');
			for (i = 0; i < FREE_FALL_PIPE_COLUMNS_COUNT; i++)
			{
				if (Shared_Buffer.Free_Fall_Obstacle_Rows[FREE_FALL_SCREEN_ROWS_COUNT - 1] & (1 << i)) Character = FREE_FALL_OBSTACLE_CHARACTER;
				else Character = ' ';
				ScreenWriteCharacter(Character);
			}
			ScreenWriteString("|\r\n"); // Display the pipe rightmost border and force scrolling
			
			// Display the current score now that the screen has been scrolled
			ScreenSetCursorLocation(FREE_FALL_SCORE_STRING_ROW, 1);
			ScreenWriteROMString(STRING_FREE_FALL_SCORE);
			itoa(Score, String_Score_Value);
			ScreenWriteString(String_Score_Value);
			
			Game_Speed_Frequency_Divider = 0;
		}
		
		// Check player collision
		i = Free_Fall_Player_Column - FREE_FALL_PLAYER_LEFTMOST_COLUMN; // Recycle i variable to compute the player's column location into the pipe
		if (Shared_Buffer.Free_Fall_Obstacle_Rows[FREE_FALL_PLAYER_ROW] & (1 << i)) Is_Player_Crashed = 1;
		// Handle player keys
		else if (KeyboardIsKeyAvailable())
		{
			switch (KeyboardReadCharacter())
			{
				// Exit game if the Escape key is pressed
				case KEYBOARD_KEY_CODE_ESCAPE:
					return;
					
				// Go to left
				case 'Q':
				case 'q':
					if (Free_Fall_Player_Column > FREE_FALL_PLAYER_LEFTMOST_COLUMN)
					{
						// Clear the player trace
						ScreenSetCursorLocation(FREE_FALL_PLAYER_ROW, Free_Fall_Player_Column);
						ScreenWriteCharacter(' '); // Clear the player trace
												
						// Display the player
						ScreenWriteCharacter(KEYBOARD_KEY_CODE_BACKSPACE); // Go left one more step
						ScreenWriteCharacter(KEYBOARD_KEY_CODE_BACKSPACE);
						ScreenWriteCharacter(FREE_FALL_PLAYER_CHARACTER);
						
						Free_Fall_Player_Column--;
					}
					break;
					
				// Go to right
				case 'D':
				case 'd':
					if (Free_Fall_Player_Column < FREE_FALL_PLAYER_RIGHTMOST_COLUMN)
					{
						// Clear the player trace
						ScreenSetCursorLocation(FREE_FALL_PLAYER_ROW, Free_Fall_Player_Column);
						ScreenWriteCharacter(' '); // Clear the player trace
						
						// Display the player
						ScreenWriteCharacter(FREE_FALL_PLAYER_CHARACTER);
						
						Free_Fall_Player_Column++;
					}
					break;
			}
		}
		
		// Display the player
		ScreenSetCursorLocation(FREE_FALL_PLAYER_ROW, Free_Fall_Player_Column);
		if (Is_Player_Crashed) ScreenSetColor(SCREEN_COLOR_CODE_TEXT_RED);
		ScreenWriteCharacter(FREE_FALL_PLAYER_CHARACTER);
		
		// The player failed, stop the game
		if (Is_Player_Crashed)
		{
			// Display the "player lost" message
			ScreenSetColor(SCREEN_COLOR_CODE_TEXT_YELLOW);
			ScreenSetCursorLocation(1, 1);
			ScreenWriteROMString(STRING_FREE_FALL_PLAYER_LOST);
			
			// Wait for the player to hit the escape key
			while (KeyboardReadCharacter() != KEYBOARD_KEY_CODE_ESCAPE);
			return;
		}
		// Increment the player score if an obstacle has been avoided, an only when the screen has been scrolled (i.e. a new obstacle row has been generated) to avoid consuming too much resources in displaying the string
		else if (Game_Speed_Frequency_Divider == 0) // The game speed frequency divider has been reset, so the screen has been scrolled 
		{
			// Increment the score if an obstacle row was successfully avoided
			if (Shared_Buffer.Free_Fall_Obstacle_Rows[FREE_FALL_PLAYER_ROW] != 0) Score++;
		}
	}
}
