/** @file System.h
 * Gather all needed includes into a single file. This is the only one file you need to include into the application.
 * @author Adrien RICCIARDI
 * @version 1.0 : 21/02/2013
 */ 
#ifndef H_SYSTEM_H
#define H_SYSTEM_H

#include <system.h> // Provide all SourceBoost functionalities

// Include only the drivers needed by the user 
#include <Driver_Flash.h>
#include <Driver_I2C.h>
#include <Driver_Internal_EEPROM.h>
#include <Driver_Random.h>
#include <Driver_Timer.h>
#include <Keyboard.h>
#include <Screen.h>
#include <Standard_Library.h>

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize all drivers and hardware stuff.
 * @warning This function must be called prior any other function.
 * @warning This function must be called only one time at the beginning of the program.
 */
void SystemInitialize(void);

/** Light the motherboard led. */
void SystemLedOn(void);

/** Turn off the motherboard led. */
void SystemLedOff(void);

/** Invert the current state of the motherboard led. */
void SystemLedToggle(void);

#endif