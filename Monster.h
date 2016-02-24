//
//  Monster.h
//  RogueLike
//
//  Created by Camden Voigt on 2/16/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#ifndef Monster_h
#define Monster_h

#include <stdio.h>

#include "binheap.h"
#include "utils.h"
#include "PlayerCharacter.h"

typedef struct position {
    int x;
    int y;
    int distance;
    binheap_node_t* heapNode;
} position_t;

/* Monster Creation */
monster_t* createMonster(room_t* rooms, int numRooms);
character_t* monsterAsCharacter(monster_t* monster, int num);

/* Monster Movement */
void moveMonster(monster_t* monster, pc_t* pc);

/* Line of Sight */
void findLineOfSightMultiple(character_t* characters, int numChars, pc_t* pc, room_t* rooms, int numRooms);
void findLineOfSightSingle(character_t* character, int numChars, pc_t* pc, room_t* rooms, int numRooms);

/* Distance to Player */
void distanceToPlayerNoTunneling(pc_t* pc, int results[][21]);
void distanceToPlayerTunneling(pc_t* pc, int results[][21]);

#endif /* Monster_h */
