/** @file System.c
 * @see System.h for description.
 * @author Adrien RICCIARDI
 */
#include <Driver_I2C.h>
#include <Driver_Random.h>
#include <Driver_UART.h>
#include <Driver_Timer.h>
#include <System.h>
#include <system.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
// Configure PIC16F876 fuses
#pragma DATA _CONFIG, _PWRTE_ON & _BODEN_ON & _WDT_OFF & _LVP_OFF & _CPD_OFF & _DEBUG_OFF & _XT_OSC & _CP_OFF

// Configure clock frequency
#pragma CLOCK_FREQ 4000000

/** The output pin connected to the led. */
#define SYSTEM_LED_PIN portb.7

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemInitialize(void)
{
	// Initialize motherboard led
	portb = 0;
	trisb = 0;
		
	// Initialize serial port
	UARTInitialize();
	// Initialize I2C module
	I2CInitialize();
	// Initialize timer0 module
	TimerInitialize();
	// Initialize random numbers generator
	RandomInitialize();
	
	// Enable interrupts
	intcon = 0xC0;
}

void SystemLedOn(void)
{
	SYSTEM_LED_PIN = 1;
}

void SystemLedOff(void)
{
	SYSTEM_LED_PIN = 0;
}

void SystemLedToggle(void)
{
	SYSTEM_LED_PIN = !SYSTEM_LED_PIN;
}