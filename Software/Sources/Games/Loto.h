/** @file Loto.h
 * Player has to bet on some numbers which are randomly drawn.
 * @author Adrien RICCIARDI
 * @version 1.0 : 03/06/2013
 */
#ifndef H_LOTO_H
#define H_LOTO_H

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------
/** How many numbers the player has to bet on. */
#define LOTO_PLAYER_NUMBERS_COUNT 6

/** Lower bound of allowed numbers. */
#define LOTO_MINIMUM_NUMBER_VALUE 1
/** Upper bound of allowed numbers. */
#define LOTO_MAXIMUM_NUMBER_VALUE 49

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** The Loto game. */
void Loto(void);

#endif
