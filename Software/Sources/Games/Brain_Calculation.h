/** @file Brain_Calculation.h
 * A game in which the player should compute a maximum amount of small calculus without mistake.
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/12/2012
 * @version 1.1 : 01/03/2013, ported to Text Games System.
 */
#ifndef H_BRAIN_CALCULATION_H
#define H_BRAIN_CALCULATION_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many correct answers you need to end the game. */
#define BRAIN_CALCULATION_WINNING_ANSWERS_COUNT 100

/** Maximum number of digits in the user's answer. */
#define MAXIMUM_DIGITS_COUNT 4

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** The Brain Calculation game. */
void BrainCalculation(void);

#endif