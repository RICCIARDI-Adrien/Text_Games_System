/** @file Wumpus.h
 * A Hunt the Wumpus game like with some customizations.
 * @author Adrien RICCIARDI
 * @version 1.0 : 15/03/2013
 * @version 1.1 : 23/03/2013, added more flexible key mapping.
 */
 #ifndef H_WUMPUS_H
 #define H_WUMPUS_H
 
//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
 /** How many bats in the cave. */
 #define BATS_COUNT 3
 /** How many pits in the cave. */
 #define PITS_COUNT 4
 
/** How many rooms from the top to the bottom of the cave. */
#define CAVE_HEIGHT 4
/** How many rooms from the left to the right of the cave. */
#define CAVE_WIDTH 5
 
 /** How many arrows the player has at the beginning of the game. */
 #define PLAYER_ARROWS_COUNT 5
 
 // All the following keys must be defined in lowercase letters
 /** Key used to move in the front room. */
 #define KEY_MOVE_UP 'z'
 /** Key used to move in the back room. */
 #define KEY_MOVE_DOWN 's'
 /** Key used to move in the left room. */
 #define KEY_MOVE_LEFT 'q'
 /** Key used to move in the right room. */
 #define KEY_MOVE_RIGHT 'd'
 
 /** Key used to shoot in the front room. */
 #define KEY_SHOOT_UP 'o'
 /** Key used to shoot in the back room. */
 #define KEY_SHOOT_DOWN 'l'
 /** Key used to shoot in the left room. */
 #define KEY_SHOOT_LEFT 'k'
 /** Key used to shoot in the right room. */
 #define KEY_SHOOT_RIGHT 'm'
 
//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
 /** Hunt the Wumpus game. */
 void Wumpus(void); 
 
 #endif