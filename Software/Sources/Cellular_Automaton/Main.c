/** @file Main.c
 * The famous Conway's game of life.
 * @author Adrien RICCIARDI
 * @version 1.0 : 12/03/2014
 */
#include <System/System.h>
#include "Cell.h"
#include "Configuration.h"
#include "World.h"

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned long Generations_Count = 0;
	
	// Initialize hardware
	SystemInitialize();
	
	// Configure terminal
	ScreenHideCursor();
	ScreenDisableScrolling();
	ScreenSetColor(SCREEN_COLOR_CODE_TEXT_GREEN);
	
	while (1)
	{
		WorldCreate();
		WorldDisplay();
		
		while (1)
		{
			WorldComputeNextGeneration();
			Generations_Count++;
			
			// Create a new world when the current one has got enough time to develop
			if (Generations_Count == CONFIGURATION_MAXIMUM_GENERATIONS_COUNT)
			{
				Generations_Count = 0;
				break;
			}
			
			// Create a new world if the user hit a key
			if (KeyboardIsKeyAvailable())
			{
				KeyboardReadCharacter();
				break;
			}
		}		
	}
}