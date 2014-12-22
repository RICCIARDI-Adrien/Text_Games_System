/** @file Driver_Timer.h
 * Provide a 60Hz timer you can use in game loops.\n
 * Timer usage :\n
 * @code
 *    while (Is_Game_Running) // Game loop
 *    {
 *        // Do game stuff
 *        ...
 *
 *        TimerWaitNextTick(); // Wait until the next tick occurs
 *    }
 * @endcode
 * @author Adrien RICCIARDI
 * @version 1.0 : 18/05/2013
 * @version 1.1 : 25/05/2013, removed TimerEnable() and TimerDisable() functions to ease use of the timer.
 * @version 1.2 : 03/07/2013, removed need for timer interruption to gain some computing cycles.
 */
#ifndef H_DRIVER_TIMER_H
#define H_DRIVER_TIMER_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize timer0 module at 61,04 Hz.
 * @warning This function is automatically called by the system during startup, you have not to call it.
 */
void TimerInitialize(void);

/** Wait for the next 60Hz tick.
 * @note This is a blocking function.
 */
void TimerWaitNextTick(void);

#endif
