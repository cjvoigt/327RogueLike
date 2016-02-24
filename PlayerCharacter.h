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
character_t* playerAsCharacter(pc_t* pc);

/* Player Movement */
void movePlayer(pc_t* pc);

#endif /* PlayerCharcter_h */
