/** @file Driver_Random.h
 * Use the analog-to-digital converter, timer 0 and timer 2 to create random numbers.
 * @author Adrien RICCIARDI
 * @version 1.0 : 28/02/2013
 */
#ifndef H_DRIVER_RANDOM_H
#define H_DRIVER_RANDOM_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize timers and ADC modules used to generate random numbers.
 * @warning This function is automatically called by the system during startup, you have not to call it.
 */
void RandomInitialize(void);

/** Initialize the pseudo-random generator with a truly random value. */
void RandomInitializeGenerator(void);

/** Generate the next pseudo-random number.
 * @return A pseudo-random number.
 */
unsigned char RandomGenerateNumber(void);

#endif