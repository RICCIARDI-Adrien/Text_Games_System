/** @file Free_Fall.h
 * The player is falling through a huge pipe and must avoid to crash on obstacles.
 * @author Adrien RICCIARDI
 */
#ifndef H_FREE_FALL_H
#define H_FREE_FALL_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The screen vertical size. */
#define FREE_FALL_SCREEN_ROWS_COUNT 24
/** The screen horizontal size. */
#define FREE_FALL_SCREEN_COLUMNS_COUNT 80

/** The pipe width in characters.
 * @note The maximum allowed value is 16.
 * @warning Don't forget to modify FREE_FALL_PIPE_WIDTH_BIT_MASK too.
 */
#define FREE_FALL_PIPE_COLUMNS_COUNT 10

/** The player row. */
#define FREE_FALL_PLAYER_ROW 3
/** The score string row. */
#define FREE_FALL_SCORE_STRING_ROW FREE_FALL_SCREEN_ROWS_COUNT
 
//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** The Free Fall game. */
void FreeFall(void);

#endif