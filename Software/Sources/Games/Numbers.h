/** @file Numbers.h
 * Find a number game.
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/02/2013
 */
#ifndef H_NUMBERS_H
#define H_NUMBERS_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Size of the number to find in digits. */
#define NUMBER_DIGITS_COUNT 5

/** How many attempts the player can do. */
#define NUMBERS_MAXIMUM_ATTEMPTS 20

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** The Numbers game. */
void Numbers(void);

#endif