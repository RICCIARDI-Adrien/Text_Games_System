/** @file Demo.c
 * @see Demo.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>"
#include "Demo.h"
#include "Shared_Buffer.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many transitions are available. */
#define TRANSITIONS_COUNT 7

/** The maximum reachable height for a falling snow column. */
#define TRANSITION_FALLING_SNOW_MAXIMUM_COLUMN_HEIGHT 20

// How many balls in the bouncing balls transition
//#define TRANSITION_BOUNCING_BALLS_COUNT 22

/** Maximum number to find for the finding number transition (must fit on a 16-bit value). */
#define TRANSITION_FINDING_NUMBER_MAXIMUM_VALUE 50000
/** How many attempts can the computer do. */
#define TRANSITION_FINDING_NUMBER_ATTEMPTS_COUNT 15
/** How many time (in seconds) to show the finding result (victory or loss). */
#define TRANSITION_FINDING_NUMBER_END_WAITING_TIME_SECONDS 4

/** How many matrix columns are handled simultaneously. */
#define TRANSITION_MATRIX_SIMULTANEOUS_COLUMNS_COUNT 5
/** How long a column grows until another column is handled (the size is in characters). */
#define TRANSITION_MATRIX_COLUMN_SIZE_BEFORE_NEW 5

// Representation of a bouncing ball
/*typedef struct
{
	unsigned char Row, Column;
	signed char Horizontal_Speed, Vertical_Speed;
	unsigned char Is_Alive; // Can't use a "bool" here because SourceBoost can't make bit arrays
} TBouncingBall;*/

/*typedef struct
{
	unsigned char Column_Height_Index; // Index of the selected column height into the columns array
	unsigned char Is_Manager_Available; // Tell if this column manager is available for handling a new column or not
} TMatrixColumnManager;*/

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Identify a selected Matrix column. */
typedef struct
{
	unsigned char Row, Column;
} TMatrixColumn;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Keep the last pressed key. */
static unsigned char Last_Pressed_Key;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Wait for 100 milliseconds x specified time and exit when the user hit a key.
 * @param Hundredth_Milliseconds How many multiples of 100 ms to wait.
 * @return true if the user hit a key or false if not.
 */
static bool Wait(unsigned char Hundredth_Milliseconds)
{
	while (Hundredth_Milliseconds > 0)
	{
		if (KeyboardIsKeyAvailable())
		{
			Last_Pressed_Key = KeyboardReadCharacter();
			return true;
		}
		delay_ms(100);
		Hundredth_Milliseconds--;
	}
	return false;
}

/** Fill the screen with characters from up to bottom. */
inline void TransitionFillScreenUpToDown(void)
{
	unsigned char Row, Column, Key;
	
	for (Row = 1; Row <= DEMO_SCREEN_HEIGHT; Row++)
	{
		for (Column = 0; Column <= DEMO_SCREEN_WIDTH / 2; Column++)
		{
			ScreenSetCursorLocation(Row, Column);
			ScreenWriteCharacter('#');
			ScreenSetCursorLocation(Row, DEMO_SCREEN_WIDTH - Column);
			ScreenWriteCharacter('#');
			
			// Exit transition if the player hit a key
			if (KeyboardIsKeyAvailable())
			{
				Last_Pressed_Key = KeyboardReadCharacter();
				return;
			}
		}
	}
}

/** Fill the screen with characters from bottom to up. */
inline void TransitionFillScreenDownToUp(void)
{
	unsigned char Row, Column;
	
	for (Row = DEMO_SCREEN_HEIGHT; Row >= 1 ; Row--)
	{
		for (Column = 0; Column <= DEMO_SCREEN_WIDTH / 2; Column++)
		{
			ScreenSetCursorLocation(Row, Column);
			ScreenWriteCharacter('#');
			ScreenSetCursorLocation(Row, DEMO_SCREEN_WIDTH - Column);
			ScreenWriteCharacter('#');
			
			// Exit transition if the player hit a key
			if (KeyboardIsKeyAvailable())
			{
				Last_Pressed_Key = KeyboardReadCharacter();
				return;
			}
		}
	}
}

/** Falling and stacking snow. */
inline void TransitionFallingSnow(void)
{
	unsigned char Row, Column, Columns_Height[DEMO_SCREEN_WIDTH], End_Row;
	unsigned short Remaining_Snowflakes = DEMO_SCREEN_WIDTH * TRANSITION_FALLING_SNOW_MAXIMUM_COLUMN_HEIGHT;
	
	// Reset heights
	bzero(Columns_Height, sizeof(Columns_Height));
	
	while (Remaining_Snowflakes > 0)
	{
		// Select next column to fill (it must not be completely filled)
		do
		{
			Column = RandomGenerateNumber() % DEMO_SCREEN_WIDTH;
		} while (Columns_Height[Column] >= TRANSITION_FALLING_SNOW_MAXIMUM_COLUMN_HEIGHT);

		// Initialize falling boundaries
		ScreenSetCursorLocation(1, Column + 1); // Screen coordinates are 1-based
		Row = 1;
		End_Row = DEMO_SCREEN_HEIGHT - Columns_Height[Column]; // Compute the number of rows to cross
		
		// Simulate the falling snowflake
		while (1)
		{
			// Show snowflake
			ScreenWriteCharacter('O');
			//delay_ms(100);
			
			// Stop falling when the top of the column is reached
			if (Row == End_Row) break;
			
			// Hide trace
			ScreenSetCursorLocation(Row, Column + 1);
			ScreenWriteCharacter(' ');
			
			// Go to lower row
			Row++;
			ScreenSetCursorLocation(Row, Column + 1);
		}
		
		// Update column height
		Columns_Height[Column]++;
		
		Remaining_Snowflakes--;
		
		// Exit transition if the player hit a key
		if (KeyboardIsKeyAvailable())
		{
			Last_Pressed_Key = KeyboardReadCharacter();
			return;
		}
	}
}

/** Bouncing balls colliding the screen's borders and themselves. */
inline void TransitionBouncingBalls(void)
{
	unsigned char i, j, Row, Column, Horizontal_Speed, Vertical_Speed, Remaining_Balls;
	//TBouncingBall Balls[TRANSITION_BOUNCING_BALLS_COUNT];
	TDemoBouncingBall *Balls = Shared_Buffer.Demo_Bouncing_Balls;
	bool Has_Collision_Happened;
	
	// Initialize all balls
	for (i = 0; i < DEMO_TRANSITION_BOUNCING_BALLS_COUNT; i++)
	{
		// Set random location
		Balls[i].Row = (RandomGenerateNumber() % DEMO_SCREEN_HEIGHT) + 1;
		Balls[i].Column = (RandomGenerateNumber() % DEMO_SCREEN_WIDTH) + 1;
		
		// Set random movement
		Balls[i].Horizontal_Speed = RandomGenerateNumber() % 2 == 1 ? 1 : -1;
		Balls[i].Vertical_Speed = RandomGenerateNumber() % 2 == 1 ? 1 : -1;
		
		// Enable ball
		Balls[i].Is_Alive = 1;
	}
	
	while (1)
	{
		// Compute bounces for alive balls
		Remaining_Balls = DEMO_TRANSITION_BOUNCING_BALLS_COUNT;
		for (i = 0; i < DEMO_TRANSITION_BOUNCING_BALLS_COUNT; i++)
		{
			// Compute dead balls count in the same time
			if (!Balls[i].Is_Alive)
			{
				Remaining_Balls--;
				continue;
			}
			
			// Use common variables to avoid arrays dereferencing code overhead (due to PIC architecture)
			Row = Balls[i].Row;
			Column = Balls[i].Column;
			Horizontal_Speed = Balls[i].Horizontal_Speed;
			Vertical_Speed = Balls[i].Vertical_Speed;
			
			// Erase ball trace
			ScreenSetCursorLocation(Row, Column);
			ScreenWriteCharacter(' ');
			
			// Compute bounces
			if (Row <= 1) Vertical_Speed = 1;
			else if (Row >= DEMO_SCREEN_HEIGHT) Vertical_Speed = -1;
			if (Column <= 1) Horizontal_Speed = 1;
			else if (Column >= DEMO_SCREEN_WIDTH) Horizontal_Speed = -1;
			
			// Update ball location
			Row += Vertical_Speed;
			Column += Horizontal_Speed;
			
			// Is the ball colliding with another ball ?
			Has_Collision_Happened = false;
			for (j = 0; j < DEMO_TRANSITION_BOUNCING_BALLS_COUNT; j++)
			{
				// Skip current ball
				if (j == i) continue;
				// Skip dead balls
				if (!Balls[j].Is_Alive) continue;
				
				// Check locations
				if ((Row != Balls[j].Row) || (Column != Balls[j].Column)) continue;
				
				// The balls are colliding, delete them (they are shown a last time)
				Balls[i].Is_Alive = 0;
				Balls[j].Is_Alive = 0;
				
				// Only two balls can collide, avoid other checks
				Has_Collision_Happened = true;
				break;
			}				
			
			// Show ball
			ScreenSetCursorLocation(Row, Column);
			if (Has_Collision_Happened) ScreenWriteCharacter('#');
			else ScreenWriteCharacter('O');
			
			// Store variables
			Balls[i].Row = Row;
			Balls[i].Column = Column;
			Balls[i].Horizontal_Speed = Horizontal_Speed;
			Balls[i].Vertical_Speed = Vertical_Speed;
		}
		
		// Quit if only 2 ball remain because it is hard for them to collide
		if (Remaining_Balls <= 2) return;
		
		// Exit transition if the player hit a key
		if (KeyboardIsKeyAvailable())
		{
			Last_Pressed_Key = KeyboardReadCharacter();
			return;
		}
	}
}

/** The computer is playing the Numbers game alone. */
inline void TransitionFindingNumber(void)
{
	unsigned short Number_To_Find, Proposed_Number, Interval_Minimum = 1, Interval_Maximum = TRANSITION_FINDING_NUMBER_MAXIMUM_VALUE;
	unsigned char Remaining_Attempts = TRANSITION_FINDING_NUMBER_ATTEMPTS_COUNT;
	
	// Show instructions
	ScreenWriteROMString(STRING_DEMO_FINDING_NUMBER_INSTRUCTIONS);
	
	// Create a random number in range
	do
	{
		Number_To_Find = (RandomGenerateNumber() << 8) | RandomGenerateNumber();
	} while ((Number_To_Find < 1) || (Number_To_Find > TRANSITION_FINDING_NUMBER_MAXIMUM_VALUE));
	
	// Show number to find
	ScreenWriteROMString(STRING_DEMO_FINDING_NUMBER_SHOW_NUMBER_TO_FIND);
	ScreenWriteUnsignedInteger(Number_To_Find);
	ScreenWriteROMString(STRING_GENERIC_NEW_LINE);
	
	// Try to find the number using the dichotomy principle
	while (Remaining_Attempts > 0)
	{
		// Compute a number in the middle of the search interval
		Proposed_Number = ((Interval_Maximum - Interval_Minimum) >> 1) + Interval_Minimum; // ((Max - Min) / 2) + Min
		
		// Show it
		ScreenWriteROMString(STRING_DEMO_FINDING_NUMBER_TRY_NUMBER);
		ScreenWriteUnsignedInteger(Proposed_Number);
		
		// Try this number
		// Right number found
		if (Proposed_Number == Number_To_Find)
		{
			ScreenWriteROMString(STRING_DEMO_FINDING_NUMBER_VICTORY);
			if (Wait(TRANSITION_FINDING_NUMBER_END_WAITING_TIME_SECONDS * 10)) return;
			return;
		}
		// Bad number found
		if (Proposed_Number < Number_To_Find)
		{
			ScreenWriteROMString(STRING_DEMO_FINDING_NUMBER_TOO_SMALL);
			// Adjust search interval
			Interval_Minimum = Proposed_Number;
		}
		else
		{
			ScreenWriteROMString(STRING_DEMO_FINDING_NUMBER_TOO_BIG);
			// Adjust search interval
			Interval_Maximum = Proposed_Number;
		}

		// Has the computer lost ?
		Remaining_Attempts--;
		if (Remaining_Attempts == 0)
		{
			ScreenWriteROMString(STRING_DEMO_FINDING_NUMBER_LOSS);
			if (Wait(TRANSITION_FINDING_NUMBER_END_WAITING_TIME_SECONDS * 10)) return;
			return;
		}
		
		// Wait to let the watcher understand what is happening
		if (Wait(20)) return;
	}
}

/** Fill the screen following a circular path. */
inline void TransitionFillScreenCircles(void)
{
	unsigned char Current_Row, Current_Column, Row_Start = 1, Row_End = DEMO_SCREEN_HEIGHT, Column_Start = 1, Column_End = DEMO_SCREEN_WIDTH;
	unsigned short Remaining_Characters = DEMO_SCREEN_WIDTH * DEMO_SCREEN_HEIGHT;
	
	do
	{
		// Draw left to right row
		Current_Row = Row_Start;
		for (Current_Column = Column_Start; Current_Column <= Column_End; Current_Column++)
		{
			ScreenSetCursorLocation(Current_Row, Current_Column);
			ScreenWriteCharacter('#');
			Remaining_Characters--;
		}
		// Upper row has been completely filled
		Row_Start++;
				
		// Draw up to down column
		Current_Column = Column_End;
		for (Current_Row = Row_Start; Current_Row <= Row_End; Current_Row++)
		{
			ScreenSetCursorLocation(Current_Row, Current_Column);
			ScreenWriteCharacter('#');
			Remaining_Characters--;
		}
		// Right side column has been completely filled
		Column_End--;
				
		// Draw right to left row
		Current_Row = Row_End;
		for (Current_Column = Column_End; Current_Column >= Column_Start; Current_Column--)
		{
			ScreenSetCursorLocation(Current_Row, Current_Column);
			ScreenWriteCharacter('#');
			Remaining_Characters--;
		}
		// Lower row has been completely filled
		Row_End--;
		
		// Draw down to up column
		Current_Column = Column_Start;
		for (Current_Row = Row_End; Current_Row >= Row_Start; Current_Row--)
		{
			ScreenSetCursorLocation(Current_Row, Current_Column);
			ScreenWriteCharacter('#');
			Remaining_Characters--;
		}
		// Left side column has been completely filled
		Column_Start++;
		
		// Exit transition if the player hit a key
		if (KeyboardIsKeyAvailable())
		{
			Last_Pressed_Key = KeyboardReadCharacter();
			return;
		}
	} while (Remaining_Characters > 0);
}

/** Vertical falling characters as in the Matrix movie. */
inline void TransitionFillScreenMatrix(void)
{
	unsigned char Is_Column_Empty[DEMO_SCREEN_WIDTH], i, Empty_Columns_Count = DEMO_SCREEN_WIDTH, Column_Index, Selector_Index, Selected_Columns_Count = 1;
	TMatrixColumn Selected_Columns[TRANSITION_MATRIX_SIMULTANEOUS_COLUMNS_COUNT]; // A non selected column has the Row value set to zero (which is not an allowed VT 100 terminal value)
		
	// Initialize the Matrix
	memset(Is_Column_Empty, 1, sizeof(Is_Column_Empty));
	ScreenSetColor(SCREEN_COLOR_CODE_TEXT_GREEN);
	
	// Select the first column
	Selected_Columns[0].Row = 1;
	i = RandomGenerateNumber() % DEMO_SCREEN_WIDTH;
	Selected_Columns[0].Column = i + 1; // VT 100 coordinates are 1-based
	Is_Column_Empty[i] = 0;
	
	// The other columns are not selected yet
	for (i = 1; i < TRANSITION_MATRIX_SIMULTANEOUS_COLUMNS_COUNT; i++) Selected_Columns[i].Row = 0;
	
	// Fill all the columns
	while (Empty_Columns_Count > 0)
	{
		// Handle selected columns
		for (i = 0; i < TRANSITION_MATRIX_SIMULTANEOUS_COLUMNS_COUNT; i++)
		{
			// Avoid non selected columns
			if (Selected_Columns[i].Row == 0) continue;
			
			// Display selected column state to screen
			ScreenSetCursorLocation(Selected_Columns[i].Row, Selected_Columns[i].Column);
			ScreenWriteCharacter((RandomGenerateNumber() % 94) + 33); // ASCII printable characters from '!' to character code 127
			
			// Grow column
			Selected_Columns[i].Row++;
			
			// Select a new column if this one has reached a specified threshold and more simultaneous columns can be handled
			if ((Selected_Columns[i].Row == TRANSITION_MATRIX_COLUMN_SIZE_BEFORE_NEW) && (Selected_Columns_Count < TRANSITION_MATRIX_SIMULTANEOUS_COLUMNS_COUNT))
			{
				// Find an empty column
				do
				{
					Column_Index = RandomGenerateNumber() % DEMO_SCREEN_WIDTH;
				} while (!Is_Column_Empty[Column_Index]);
				
				// Find an empty column selector
				for (Selector_Index = 0; Selector_Index < TRANSITION_MATRIX_SIMULTANEOUS_COLUMNS_COUNT; Selector_Index++)
				{
					if (Selected_Columns[Selector_Index].Row == 0) break;
				}
				
				// Select this column
				Selected_Columns[Selector_Index].Row = 1;
				Selected_Columns[Selector_Index].Column = Column_Index + 1; // VT 100 coordinates are 1-based
				Is_Column_Empty[Column_Index] = 0;
				Selected_Columns_Count++;
			}
			
			// Recycle column selector if its column has reached the screen bottom
			else if (Selected_Columns[i].Row > DEMO_SCREEN_HEIGHT)
			{
				// Can't select an empty column when all have been selected yet
				if (Empty_Columns_Count <= TRANSITION_MATRIX_SIMULTANEOUS_COLUMNS_COUNT)
				{
					// Stop column growing
					Selected_Columns[i].Row = 0;
					Empty_Columns_Count--;
				}
				// There are still empty columns to select
				else
				{
					// Find an empty column
					do
					{
						Column_Index = RandomGenerateNumber() % DEMO_SCREEN_WIDTH;
					} while (!Is_Column_Empty[Column_Index]);
					
					// Select this column
					Selected_Columns[i].Row = 1;
					Selected_Columns[i].Column = Column_Index + 1; // VT 100 coordinates are 1-based
					Is_Column_Empty[Column_Index] = 0;
					Empty_Columns_Count--;
				}
			}
			
			// Exit transition if the player hit a key
			if (KeyboardIsKeyAvailable())
			{
				ScreenSetColor(SCREEN_COLOR_CODE_TEXT_WHITE); // Reset color attribute
				Last_Pressed_Key = KeyboardReadCharacter();
				return;
			}
		}
		delay_ms(100);
	}
	ScreenSetColor(SCREEN_COLOR_CODE_TEXT_WHITE); // Reset color attribute
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void Demo(bool Is_Random_Mode_Enabled)
{
	unsigned char Transition_Number = TRANSITIONS_COUNT - 1; // To start from 0 when the sequence mode is started
	
	// Initialize drawing area
	ScreenDisableScrolling();
	ScreenHideCursor();
	
	// Reinitialize the variable each time the demo is called, or it will contain the last hit key, which may be the exiting character...
	Last_Pressed_Key = 0;

	while (1)
	{
		// Wait 750 ms
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		
		// Prepare transition
		ScreenClear();
		RandomInitializeGenerator();
		
		// Choose transition according to mode (random or sequence)
		if (Is_Random_Mode_Enabled) Transition_Number = RandomGenerateNumber() % TRANSITIONS_COUNT;
		else
		{
			Transition_Number++;
			if (Transition_Number == TRANSITIONS_COUNT) Transition_Number = 0; // Continue from beginning when the end is reached
		}
		
		// Show selected transition
		switch (Transition_Number)
		{
			case 0:
				TransitionFillScreenUpToDown();
				break;
			case 1:
				TransitionFillScreenDownToUp();
				break;
			case 2:
				TransitionFillScreenCircles();
				break;
			case 3:
				TransitionFillScreenMatrix();
				break;
			case 4:
				TransitionFallingSnow();
				break;
			case 5:
				TransitionBouncingBalls();
				break;
			case 6:
				TransitionFindingNumber();
				break;
		}
		
		// Change transition or quit program according to pressed key
		if (Last_Pressed_Key == KEYBOARD_KEY_CODE_ESCAPE)
		{
			ScreenEnableScrolling();
			ScreenShowCursor();
			return;
		}
	}
}
	