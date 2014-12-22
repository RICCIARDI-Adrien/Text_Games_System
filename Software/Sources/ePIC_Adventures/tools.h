/****************************************/
/*			+-----	  / \    .------	*/
/*          |      \  |||   /			*/
/*     ___  |       | |||  |			*/
/*    /	__\ |______/  |||  |			*/
/*    \___. |        -----  \			*/
/*			|         +|+    *------	*/
/*                     |				*/
/*                     *				*/
/*	/\  |\ \  / == |\| --- | | |\ == /  */
/* /  \ |/  \/  -- | |  |  |_| |\ -- /  */
/*										*/
/*	ePIC Adventures Project				*/
/*	Only for Strawberry PIC				*/
/*										*/
/*	File name: 	tools.c					*/
/*	Author:		Denis CARLUS			*/
/*	Date:		23/06/2013				*/
/*	Version:	1.0						*/
/****************************************/


#ifndef __TOOLS_FUN__
#define __TOOLS_FUN__

#define STRAWBERRY_PIC_UPGRADED

// Used for better readability
#define POWER2_2		1
#define POWER2_4		2
#define POWER2_8		3
#define POWER2_16		4
#define POWER2_32		5
#define POWER2_64		6
#define POWER2_128		7

/** @brief A memory byte used for random values. */
unsigned char tmp ;

/**
 * @brief Compute a value modulo a power of 2.
 * @param 	value		The value to be divided.
 * @param 	powermod	The power 2 of the modulo.
 * @return 	The value of the remaining of the division.
 * @warning Returns 0 if powermod is strictly greater than 7.
 */
unsigned char modulo(unsigned char value, unsigned char powermod) {
	if (powermod < 8)
		return (value & ((1 << powermod) - 1)) ;
	else
		return 0 ;
}

/**
 * @brief Compute a value modulo 2.
 * @param 	value		The given value.
 * @return 	0 if the given value is an even one, 1 if it is an odd value.
 */
#define modulo2(value) (value & 0b00000001)

/**
 * @brief Get the even value of the given one according to bits.
 *		  For example, it returns 6 if 7 is put as parameter :
 *		  <pre>
 *		  7: 0b00000111 & 0b11111110 = 0b00000110 (6)
 *		  6: 0b00000110 & 0b11111110 = 0b00000110 (6)
 *		  </pre>
  * @return The previous even value of the given one if odd, the same value if already even.
 */
 #define lower_even(value) (value & 0b11111110)

#endif