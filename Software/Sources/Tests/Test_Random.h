/** @file Test_Random.h
 * Test the efficiency of the random numbers generator.
 * @author Adrien RICCIARDI
 * @version 1.0 : 01/04/2013
 */
#ifndef H_TEST_RANDOM_H
#define H_TEST_RANDOM_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many numbers to generate (maximum value is 65535). */
#define TEST_RANDOM_NUMBERS_COUNT 32768

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** The random numbers generator test. */
void TestRandom(void);
 
#endif