/** @file Driver_Random.c
 * @see Driver_Random.h for description.
 * @author Adrien RICCIARDI
 */ 
#include <Driver_Random.h>
#include <system.h>

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
// Seed of the random number generator
static unsigned char Random_Seed;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Sample a byte from the ADC module. The ADC channel pin has been left floating to make antenna and get random values.
 * @return The sampled random value.
 */
inline unsigned char RandomADCReadByte(void)
{
	// Start conversion
	adcon0.GO = 1;
	
	// Wait for conversion to finish
	while (adcon0.GO);
	return adresl;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void RandomInitialize(void)
{
	// Initialize ADC module
	trisa = 1; // Set RA0 only as ADC input
	adcon1 = 0x8E; // Result of conversion is left justified
	adcon0 = 0x41; // Conversion clock at Fosc / 8 (TAD = 2µs), ADC module enabled
	
	// Start timer 2 with high frequency (timer 0 is started by TimerInitialize())
	t2con = 0x04; // Start timer 2 with prescaler of 1
}

void RandomInitializeGenerator(void)
{
	Random_Seed = RandomADCReadByte() ^ tmr0;
}

unsigned char RandomGenerateNumber(void)
{
	Random_Seed = (((Random_Seed << 1) + Random_Seed) - 7) ^ tmr2; // New_Seed = (Previous_Seed * 3 - 7) XOR Arbitrary_Value
	return Random_Seed;
}
