/** @file Demo.h
 * A demo program to show Text Games System amazing and enormous power.
 * @author Adrien RICCIARDI
 * @version 1.0 : 03/06/2013
 */
#ifndef H_DEMO_H
#define H_DEMO_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many balls in the bouncing balls transition. */
#define DEMO_TRANSITION_BOUNCING_BALLS_COUNT 22

/** Width of the demo screen in characters. */
#define DEMO_SCREEN_WIDTH 80
/** Height of the demo screen in characters. */
#define DEMO_SCREEN_HEIGHT 24

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** Representation of a bouncing ball.
 * This definition is located here because it is needed by the shared buffer.
 */
typedef struct
{
	unsigned char Row; //! Ball row location.
	unsigned char Column; //! Ball column location.
	signed char Horizontal_Speed; //! Ball current horizontal speed.
	signed char Vertical_Speed; //! Ball current vertical speed.
	unsigned char Is_Alive; //! Can't use a "bool" here because SourceBoost can't make bit arrays.
} TDemoBouncingBall;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** The demonstration.
 * @param Is_Random_Mode_Enabled Set to true to select randomly the next transition or set to false to show all transitions in sequence.
 */
void Demo(bool Is_Random_Mode_Enabled);

#endif