/** @file Test_Random.c
 * @see Test_Random.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Test_Random.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Gather near numbers into sub-intervals. */
#define TEST_RANDOM_SUB_INTERVALS_COUNT 32

/** How many numbers are in a specific sub-interval. */
#define TEST_RANDOM_SUB_INTERVAL_NUMBERS_COUNT (256 / TEST_RANDOM_SUB_INTERVALS_COUNT)

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The random interval [0..255] is divided in 32 sub-intervals to make the test fit in RAM. */
static unsigned short Results[TEST_RANDOM_SUB_INTERVALS_COUNT];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void TestRandom(void)
{
	unsigned short Counter, Maximum_Value = 0, Minimum_Value = 0xFFFF, Value;
	unsigned char Number, i;
	
	ScreenWriteString("Starting random test with ");
	ScreenWriteUnsignedInteger(TEST_RANDOM_NUMBERS_COUNT);
	ScreenWriteString(" values...\r\n");
	
	// Initialize results array
	for (i = 0; i < TEST_RANDOM_SUB_INTERVALS_COUNT; i++) Results[i] = 0;
	
	// Do test
	RandomInitializeGenerator();
	for (Counter = 0; Counter < TEST_RANDOM_NUMBERS_COUNT; Counter++)
	{
		// Generate number
		Number = RandomGenerateNumber();
		Number /= TEST_RANDOM_SUB_INTERVAL_NUMBERS_COUNT;
		Results[Number]++;
		
		// Compute maximum and minimum in the same time
		Value = Results[Number];
		if (Value > Maximum_Value) Maximum_Value = Value;
		if (Value < Minimum_Value) Minimum_Value = Value;
	}
	
	// Show results
	for (i = 0; i < TEST_RANDOM_SUB_INTERVALS_COUNT; i++)
	{
		// Show interval
		ScreenWriteCharacter('[');
		ScreenWriteUnsignedInteger(i * TEST_RANDOM_SUB_INTERVAL_NUMBERS_COUNT);
		ScreenWriteString("..");
		ScreenWriteUnsignedInteger((i * TEST_RANDOM_SUB_INTERVAL_NUMBERS_COUNT) + 7);
		ScreenWriteString("] : ");
		
		// Show values
		ScreenWriteUnsignedInteger(Results[i]);
		
		// TODO : show bar graph
		ScreenWriteString("\r\n");
	}
	
	ScreenWriteString("\r\nHit a key to return to main menu.");
	KeyboardReadCharacter();
}
 