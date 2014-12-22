/** @file Driver_Timer.c
 * @see Driver_Timer.h for description.
 * @author Adrien RICCIARDI
 */
#include <Driver_Timer.h>
#include <system.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void TimerInitialize(void)
{
	option_reg = 0x85; // Fosc / 4 source frequency, prescaler of 64
}

void TimerWaitNextTick(void)
{
	while (!intcon.T0IF);
	intcon.T0IF = 0;
}