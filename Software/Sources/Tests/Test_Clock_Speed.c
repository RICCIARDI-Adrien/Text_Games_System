/** @file Test_Clock_Speed.c
 * @see Test_Clock_Speed.h for description.
 */
#include <System/System.h>
#include "Test_Clock_Speed.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void TestClockSpeed(void)
{
	// The internal clock frequency (Finst) is equal to Fosc / 4
	// The test is to toggle the state of the led pin to monitor this frequency with an oscilloscope. The blinking frequency is Finst / 6 (see code below)
	// The real oscillator frequency will be led pin frequency * 24
	
	// The following string needs to be split in two parts to avoid using too much RAM on the PIC16F876
	ScreenWriteString("The LED pin will oscillate indefinitely at Fosc / 24 frequency.\r\n");
	ScreenWriteString("You have to reboot the system to stop this test.\r\n");
	
	// The test is written in assembly to avoid most of compiler interferences (but not all)
	asm
	{
	Loop:
		// Set signal low
		bcf _portb, 7 // 1 cycle
		nop // 1 cycle
		
		// Set signal high
		bsf _portb, 7 // 1 cycle
		goto Loop // 2-cycle instruction
	}
}
 