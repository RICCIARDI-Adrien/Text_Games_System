/** @file World.h
 * Handle the whole cells world.
 * @author Adrien RICCIARDI
 * @version 1.0 : 12/03/2014
 */
#ifndef H_WORLD_H
#define H_WORLD_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Create a new random world. */
void WorldCreate(void);

/** Show the current state of the world. */
void WorldDisplay(void);

/** Compute the next state of each cells and display them. */
void WorldComputeNextGeneration(void);

#endif