//
//  PlayerCharcter.h
//  RogueLike
//
//  Created by Camden Voigt on 2/3/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#ifndef PlayerCharacter_h
#define PlayerCharacter_h

#include <stdio.h>
#include <stdint.h>

#include "utils.h"
#include "binheap.h"
#include "Dungeon.h"

/* Player Creation */
pc_t* createPlayerCharacter(room_t* rooms);
character_t  playerAsCharacter(pc_t* pc);
void changePos(pc_t* pc, room_t* rooms);

/* Player Movement */
void movePlayer(pc_t* pc, int direction);
void swapPlayer(pc_t* pc, int newX, int newY); 

#endif /* PlayerCharcter_h */
