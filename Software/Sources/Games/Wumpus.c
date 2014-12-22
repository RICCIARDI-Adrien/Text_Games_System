/** @file Wumpus.c
 * @see Wumpus.h for description.
 * @author Adrien RICCIARDI
 */
#include <System/System.h>
#include "Strings.h"
#include "Wumpus.h"

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Define DEBUG symbol to see debugging informations, comment out to save program and RAM space. */
//#define DEBUG

/** Indicate if bats are present or not in a room. */
#define BATS_PRESENCE 1
/** Indicate if a pit is present or not in a room. */
#define PIT_PRESENCE 2
/** How many rooms in the whole cave. */
#define ROOMS_TOTAL_COUNT (CAVE_HEIGHT * CAVE_WIDTH)

/** Tell if the player can go in the upper direction or not. */
#define PLAYER_CAN_GO_UP (Player_Row > 0)
/** Tell if the player can go in the downer direction or not. */
#define PLAYER_CAN_GO_DOWN (Player_Row < CAVE_HEIGHT - 1)
/** Tell if the player can go in the left direction or not. */
#define PLAYER_CAN_GO_LEFT (Player_Column > 0)
/** Tell if the player can go in the right direction or not. */
#define PLAYER_CAN_GO_RIGHT (Player_Column < CAVE_WIDTH - 1)

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All rooms in the cave. */
static unsigned char Rooms[CAVE_HEIGHT * CAVE_WIDTH];
/** Characters location and arrows. */
static unsigned char Player_Row, Player_Column, Arrows_Count, Wumpus_Room, Player_Location;
/** Temporary byte shared by all functions to save memory. */
static unsigned char Temp_Byte;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/* Initialize the cave. */
inline void CaveInitialize(void)
{
	unsigned char i;
	
	// Must be initialized every time as the global variables are initialized only during whole program startup
	Arrows_Count = PLAYER_ARROWS_COUNT;
	
	// Initialize cave
	for (i = 0; i < ROOMS_TOTAL_COUNT; i++) Rooms[i] = 0;
	
	// Put bats into the cave
	for (i = 0; i < BATS_COUNT; i++)
	{
		// Choose a free room
		do
		{
			Temp_Byte = RandomGenerateNumber() % ROOMS_TOTAL_COUNT;
		} while (Rooms[Temp_Byte] != 0);
		Rooms[Temp_Byte] = BATS_PRESENCE;
		
		#ifdef DEBUG
			ScreenWriteString("[DEBUG] Bats ");
			ScreenWriteUnsignedInteger(i);
			ScreenWriteString(" room location : ");
			ScreenWriteUnsignedInteger(Temp_Byte);
			ScreenWriteString("\r\n");
		#endif
	}
	
	// Put pits into the cave where there is no bat
	for (i = 0; i < PITS_COUNT; i++)
	{
		// Choose a free room
		do
		{
			Temp_Byte = RandomGenerateNumber() % ROOMS_TOTAL_COUNT;
		} while (Rooms[Temp_Byte] != 0);
		Rooms[Temp_Byte] = PIT_PRESENCE;
		
		#ifdef DEBUG
			ScreenWriteString("[DEBUG] Pit ");
			ScreenWriteUnsignedInteger(i);
			ScreenWriteString(" room location : ");
			ScreenWriteUnsignedInteger(Temp_Byte);
			ScreenWriteString("\r\n");
		#endif
	}
	
	// Put the Wumpus in any room, it don't get worried by pits or bats
	Wumpus_Room = RandomGenerateNumber() % ROOMS_TOTAL_COUNT;
		
	// Put the player in a free room
	do
	{
		Player_Location = RandomGenerateNumber() % ROOMS_TOTAL_COUNT;
	} while ((Rooms[Player_Location] != 0) || (Player_Location == Wumpus_Room));
	Player_Row = Player_Location / CAVE_WIDTH;
	Player_Column = Player_Location - (Player_Row * CAVE_WIDTH);
}

/* Show informations about game and player. */
inline void ShowHUD(void)
{
	bool Is_First_Displayed_Room = true; // Useful to determinate if we must draw a semi colon separator or not when displaying reachable rooms
	bool Are_Bats_Nearby = false, Is_Pit_Nearby = false, Is_Wumpus_Nearby = false;
	
	// Show room number
	ScreenWriteROMString(STRING_WUMPUS_HUD_ROOM_NUMBER);
	ScreenWriteUnsignedInteger(Player_Location);
	
	// Show arrows left count
	ScreenWriteROMString(STRING_WUMPUS_HUD_ARROWS_LEFT_1);
	ScreenWriteUnsignedInteger(Arrows_Count);
	ScreenWriteROMString(STRING_WUMPUS_HUD_ARROWS_LEFT_2);
	
	// Show available rooms
	// Upside room
	if (PLAYER_CAN_GO_UP)
	{
		// Compute upside room location
		Temp_Byte = Player_Location - CAVE_WIDTH;
		ScreenWriteROMString(STRING_WUMPUS_HUD_CAN_GO_UPSIDE_ROOM);
		ScreenWriteUnsignedInteger(Temp_Byte);
		Is_First_Displayed_Room = false;
		
		// Check entities closeness
		if (Temp_Byte == Wumpus_Room) Is_Wumpus_Nearby = true;
		Temp_Byte = Rooms[Temp_Byte];
		if (Temp_Byte == BATS_PRESENCE) Are_Bats_Nearby = true;
		if (Temp_Byte == PIT_PRESENCE) Is_Pit_Nearby = true;
	}
	// Downside room
	if (PLAYER_CAN_GO_DOWN)
	{
		// Compute downside room location
		Temp_Byte = Player_Location + CAVE_WIDTH;
		// Show separator if needed
		if (!Is_First_Displayed_Room) ScreenWriteROMString(STRING_WUMPUS_HUD_ROOMS_DISPLAYING_SEPARATOR);
		ScreenWriteROMString(STRING_WUMPUS_HUD_CAN_GO_DOWNSIDE_ROOM);
		ScreenWriteUnsignedInteger(Temp_Byte);
		Is_First_Displayed_Room = false;
		
		// Check entities closeness
		if (Temp_Byte == Wumpus_Room) Is_Wumpus_Nearby = true;
		Temp_Byte = Rooms[Temp_Byte];
		if (Temp_Byte == BATS_PRESENCE) Are_Bats_Nearby = true;
		if (Temp_Byte == PIT_PRESENCE) Is_Pit_Nearby = true;
	}
	// Leftside room
	if (PLAYER_CAN_GO_LEFT)
	{
		// Compute downside room location
		Temp_Byte = Player_Location - 1;
		// Show separator if needed
		if (!Is_First_Displayed_Room) ScreenWriteROMString(STRING_WUMPUS_HUD_ROOMS_DISPLAYING_SEPARATOR);
		ScreenWriteROMString(STRING_WUMPUS_HUD_CAN_GO_LEFTSIDE_ROOM);
		ScreenWriteUnsignedInteger(Temp_Byte);
		Is_First_Displayed_Room = false;
		
		// Check entities closeness
		if (Temp_Byte == Wumpus_Room) Is_Wumpus_Nearby = true;
		Temp_Byte = Rooms[Temp_Byte];
		if (Temp_Byte == BATS_PRESENCE) Are_Bats_Nearby = true;
		if (Temp_Byte == PIT_PRESENCE) Is_Pit_Nearby = true;
	}
	// Rightside room
	if (PLAYER_CAN_GO_RIGHT)
	{
		// Compute downside room location
		Temp_Byte = Player_Location + 1;
		// Show separator if needed
		if (!Is_First_Displayed_Room) ScreenWriteROMString(STRING_WUMPUS_HUD_ROOMS_DISPLAYING_SEPARATOR);
		ScreenWriteROMString(STRING_WUMPUS_HUD_CAN_GO_RIGHTSIDE_ROOM);
		ScreenWriteUnsignedInteger(Temp_Byte);
		
		// Check entities closeness
		if (Temp_Byte == Wumpus_Room) Is_Wumpus_Nearby = true;
		Temp_Byte = Rooms[Temp_Byte];
		if (Temp_Byte == BATS_PRESENCE) Are_Bats_Nearby = true;
		if (Temp_Byte == PIT_PRESENCE) Is_Pit_Nearby = true;
	}
	ScreenWriteROMString(STRING_GENERIC_NEW_LINE);
	
	// Show entities closeness
	if (Are_Bats_Nearby) ScreenWriteROMString(STRING_WUMPUS_HUD_BATS_NEARBY);
	if (Is_Pit_Nearby) ScreenWriteROMString(STRING_WUMPUS_HUD_PIT_NEARBY);
	if (Is_Wumpus_Nearby) ScreenWriteROMString(STRING_WUMPUS_HUD_WUMPUS_NEARBY);
	
	// Show prompt
	ScreenWriteROMString(STRING_WUMPUS_HUD_PROMPT);
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void Wumpus(void)
{
	unsigned char Key;
	bool Has_Player_Hit_Wall; // Tell if the player has shot into a wall or not
	
	// Show title
	ScreenWriteROMString(STRING_WUMPUS_TITLE);
	
	// Initialize cave
	RandomInitializeGenerator();
	CaveInitialize();
	
	while (1)
	{
		#ifdef DEBUG
			ScreenWriteString("[DEBUG] Player row = ");
			ScreenWriteUnsignedInteger(Player_Row);
			ScreenWriteString(" ; Player column = ");
			ScreenWriteUnsignedInteger(Player_Column);
			ScreenWriteString("\r\n");
		#endif
		
		#ifdef DEBUG
			ScreenWriteString("[DEBUG] Wumpus room location : ");
			ScreenWriteUnsignedInteger(Wumpus_Room);
			ScreenWriteString("\r\n");
		#endif
		
		// Reset variables
		Has_Player_Hit_Wall = false;
		
		// Compute player's room location (need to be done before calling ShowHUD())
		Player_Location = Player_Row * CAVE_WIDTH + Player_Column;
		
		ShowHUD();
		
		// Handle keys
		Key = KeyboardReadCharacter();
		switch (Key)
		{
			// Escape key
			case 27: 
				return;
				
			// Move up
			case KEY_MOVE_UP:
				// The player can't exit the cave
				if (Player_Row == 0) continue;
				Player_Row--;
				goto Player_Moved;
				
			// Move down
			case KEY_MOVE_DOWN:
				// The player can't exit the cave
				if (Player_Row == CAVE_HEIGHT - 1) continue;
				Player_Row++;
				goto Player_Moved;
				
			// Move left
			case KEY_MOVE_LEFT:
				// The player can't exit the cave
				if (Player_Column == 0) continue;
				Player_Column--;
				goto Player_Moved;
				
			// Move right
			case KEY_MOVE_RIGHT:
				// The player can't exit the cave
				if (Player_Column == CAVE_WIDTH - 1) continue;
				Player_Column++;
				goto Player_Moved;
				
			// Shoot up
			case KEY_SHOOT_UP:
				// Did the player shot into a wall ?
				if (!PLAYER_CAN_GO_UP) Has_Player_Hit_Wall = true;
				// Compute room number in which the arrow is thrown
				else Temp_Byte = Player_Location - CAVE_WIDTH;
				goto Player_Shot;
				
			// Shoot down
			case KEY_SHOOT_DOWN:
				// Did the player shot into a wall ?
				if (!PLAYER_CAN_GO_DOWN) Has_Player_Hit_Wall = true;
				// Compute room number in which the arrow is thrown
				else Temp_Byte = Player_Location + CAVE_WIDTH;
				goto Player_Shot;
				
			case KEY_SHOOT_LEFT:
				// Did the player shot into a wall ?
				if (!PLAYER_CAN_GO_LEFT) Has_Player_Hit_Wall = true;
				// Compute room number in which the arrow is thrown
				else Temp_Byte = Player_Location - 1;
				goto Player_Shot;
				
			case KEY_SHOOT_RIGHT:
				// Did the player shot into a wall ?
				if (!PLAYER_CAN_GO_RIGHT) Has_Player_Hit_Wall = true;
				// Compute room number in which the arrow is thrown
				else Temp_Byte = Player_Location + 1;
				goto Player_Shot;
			
			// Unknown key
			default:
				continue;
		}

		//-------------------------------------------------------------------------
		// Handle player's movement
		//-------------------------------------------------------------------------
	Player_Moved:
		// Is there an entity in the room the player gone ?
		Player_Location = Player_Row * CAVE_WIDTH + Player_Column;
		
		// The Wumpus eats the player !
		if (Player_Location == Wumpus_Room)
		{
			ScreenWriteROMString(STRING_WUMPUS_EATS_PLAYER);
			goto End_Game_Lost;
		}
		
		// Get new room content
		Temp_Byte = Rooms[Player_Location];
		// The player falls in a pit
		if (Temp_Byte == PIT_PRESENCE)
		{
			ScreenWriteROMString(STRING_WUMPUS_PLAYER_FALLEN_INTO_PIT);
			goto End_Game_Lost;
		}
		
		// Bats throw the player to another safe location
		if (Temp_Byte == BATS_PRESENCE)
		{
			ScreenWriteROMString(STRING_WUMPUS_BATS_MOVED_PLAYER);
			
			// Put the player in a free room
			do
			{
				Player_Location = RandomGenerateNumber() % ROOMS_TOTAL_COUNT;
			} while ((Rooms[Player_Location] != 0) || (Player_Location == Wumpus_Room));
			Player_Row = Player_Location / CAVE_WIDTH;
			Player_Column = Player_Location - (Player_Row * CAVE_WIDTH);
		}
		continue;
		
		//-------------------------------------------------------------------------
		// Handle player's shot
		//-------------------------------------------------------------------------
	Player_Shot:
		Arrows_Count--;
		// Has the player hit a wall ?
		if (Has_Player_Hit_Wall) ScreenWriteROMString(STRING_WUMPUS_PLAYER_HIT_WALL);
		// No, so Temp_Byte holds the room in which the arrow went
		else
		{
			// Is the Wumpus hit ?
			if (Temp_Byte == Wumpus_Room)
			{
				ScreenWriteROMString(STRING_WUMPUS_PLAYER_KILLED_WUMPUS);
				KeyboardReadCharacter();
				return;
			}
			// Lost arrow...
			else ScreenWriteROMString(STRING_WUMPUS_ARROW_LOST);				
		}
				
		// Kill the player if he has no arrow left (this must be checked now to allow the player to kill the Wumpus with its last arrow)
		if (Arrows_Count == 0)
		{
			ScreenWriteROMString(STRING_WUMPUS_EATS_PLAYER_WITHOUT_AMMO);
			goto End_Game_Lost;
		}
		
		// Move the Wumpus
		ScreenWriteROMString(STRING_WUMPUS_MOVE_WUMPUS_AFTER_SHOT);
		
		// Put the Wumpus in a random room
		Wumpus_Room = RandomGenerateNumber() % ROOMS_TOTAL_COUNT;
		// Is this room the one where the player is located ?
		if (Wumpus_Room == Player_Location)
		{
			ScreenWriteROMString(STRING_WUMPUS_EATS_FOUND_PLAYER);
			goto End_Game_Lost;
		}	
		continue;
	}
	
// End of the game, player is dead
End_Game_Lost:
	ScreenWriteROMString(STRING_WUMPUS_GAME_LOST);
	KeyboardReadCharacter();
}