/****************************************/
/*			+-----	  /|\    .------	*/
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
/*	File name: 	main.c					*/
/*	Author:		Denis CARLUS			*/
/*	Date:		23/06/2013				*/
/*	Version:	1.0						*/
/****************************************/

#include <System/System.h>
#include "characters.h"
#include "interactions.h"
#include "gui.h"
#include "items.h"
#include "Strings.h"
#include "tools.h"

#define STRAWBERRY_PIC_UPGRADED

extern unsigned char tmp ;

/** @brief Main function, start of the program... */
int main() {
	// Initialization of the system
	SystemInitialize() ;
	RandomInitializeGenerator();

	// Initialize the world map
	initWorldMap() ;

	while (true) {
		// Ask the player create its character
		character_wizard() ;

		// Set up the GUI of the game
		ScreenClear() ;
		setupGUI() ;

		// Let's start the game, yeah!
		while (playerOne.hp_cur > 0) {
			TimerWaitNextTick() ;
																					/** MAP TRAVEL AND ITEM DETECTION **/
			// Make the player act (move or get an item)
			playerActionsOnMap() ;
			// Detect item on the ground and get it
			detectItem() ;

																					/** FIGHTING **/
			// Generate an encounter an enemy with a probability of 1/4
			RandomInitializeGenerator();
			tmp = RandomGenerateNumber() ;
			//ScreenWriteUnsignedInteger((unsigned int) tmp) ;
			if (modulo(tmp, POWER2_4) == 0) {
				// Create the monster
				createEnemy() ;

				// Update the GUI to go on Fight View
				updateToFightView()  ;

				// Fight the monsters while they are alive (or the player dies...)
				while ((enemy.hp_cur > 0) && (playerOne.hp_cur > 0) && tmp != 123) {
					TimerWaitNextTick() ;

					// Make the VGA filled for the characters
					if ((255 - playerOne.vga) < playerOne.vel) playerOne.vga = 255 ;
					else playerOne.vga += playerOne.vel ;
					updateVGA() ;

					if ((255 - enemy.vga) < enemy.vel) enemy.vga = 255 ;
					else enemy.vga += enemy.vel ;

					// The monster tries to attack
					if (enemy.vga == 255) {
						enemy.vga = 0 ;

						#ifdef STRAWBERRY_PIC_UPGRADED
						// The monster can miss its attack
						if (modulo(RandomGenerateNumber(), POWER2_128) < enemy.prc) {
						#endif
							// Not missed ! Compute the damages made to the player
							if (enemy.str < playerOne.def) {
								tmp = 1 ;
							}
							else {
								tmp = enemy.str - playerOne.def ;
							}

							#ifdef STRAWBERRY_PIC_UPGRADED
							// Critical hit comes with a chance of 5% (13 ~= 5% of 255)
							if (RandomGenerateNumber() < 13) {
								// Critical hit = double damages
								tmp = tmp + tmp ;
							}
							#endif

							// Player is damaged
							if (playerOne.hp_cur > tmp) {
								playerOne.hp_cur -= tmp ;
							}
							else {
								playerOne.hp_cur = 0 ;
							}

							// Display the message of the attack
							updatePlayerHP() ;
							writePlayerAction(FIGHT_DAMAGED) ;
						#ifdef STRAWBERRY_PIC_UPGRADED
						}
						else {
							// Missed, just print a sentence to signal it...
							writeAction(getEnemyName(), FIGHT_MISSED) ;
						}
						#endif
					}

					// The player can choose the action to do when ready...
					if (playerOne.vga == 255) {
						playerOne.vga = 0 ;
						chooseAction() ;
					}
				}

				#ifdef STRAWBERRY_PIC_UPGRADED
				if (tmp != 123) {
				#endif
																					/** END OF FIGHT: PLAYER'S VICTORY **/
					// Distribute experience points
					if (enemy.hp_cur == 0) {
						tmp = enemy.str + enemy.def ;
						// Level up if the experience is full !
						if ((tmp > (playerOne.max_exp - playerOne.exp))
								&& (playerOne.level < 100)
								/*&& (randomDistribution() != 0)*/) {
							playerOne.level++ ;
							playerOne.exp = tmp - playerOne.max_exp ;
							if ((255 - playerOne.max_exp) > 3)
								playerOne.max_exp += 3 ;
							else
								playerOne.max_exp = 255 ;

							#ifdef STRAWBERRY_PIC_UPGRADED
							// Go to level up display and distribute points
							updateToLevelUpView() ;
							distributeLevelUpPoints() ;
							#endif

							// Restore the screen display
							ScreenClear() ;
							setupGUI() ;
						}
						else {
							playerOne.exp += tmp ;
						}
					}
				#ifdef STRAWBERRY_PIC_UPGRADED
				}
				else {
																					/** END OF FIGHT: PLAYER FLEES... **/
					writePlayerAction(FIGHT_FLEE) ;
				}
				#endif

				// Finally, set back the GUI to the "Pacific View"
				updateToPacificView() ;
			}
		}

		// GAME OVER!
		writePlayerAction(FIGHT_KO) ;
		#ifdef STRAWBERRY_PIC_UPGRADED
		// Wait the player presses a key, no matter what!
		KeyboardReadCharacter() ;
		#endif

		// LOSE LEVELS BUT NOT INVENTORY!
		#ifdef STRAWBERRY_PIC_UPGRADED
		addPotion(MIN_POTION) ;
		addPotion(MIN_POTION) ;
		addPotion(MINPLUS_POTION) ;
		#endif
	}
}
