/** @file Shared_Buffer.h
 * Put here each "big" buffer which can't be concurrently accessed at the same time as the other one are.
 * This save a lot of RAM because the same memory area is used for all buffers.
 * Be careful ! If you access two different buffers in the same time the results are unpredictable.
 * @author Adrien RICCIARDI
 * @version 1.0 : 22/06/2013
 */
#ifndef H_SHARED_BUFFER_H
#define H_SHARED_BUFFER_H

#include "Demo.h"
#include "Free_Fall.h"

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** Contain all the buffers which share the same memory area. */
typedef union
{
	TDemoBouncingBall Demo_Bouncing_Balls[DEMO_TRANSITION_BOUNCING_BALLS_COUNT];
	unsigned short Free_Fall_Obstacle_Rows[FREE_FALL_SCREEN_ROWS_COUNT];
} TSharedBuffer;

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** Access to the shared buffer from other files by using this variable. */
extern TSharedBuffer Shared_Buffer;

#endif