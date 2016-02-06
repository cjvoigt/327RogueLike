//
//  PlayerCharcter.h
//  RogueLike
//
//  Created by Camden Voigt on 2/3/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#ifndef PlayerCharacter_h
#define PlayerCharacter_h

#define INFINITY 255

#include <stdio.h>
#include <stdint.h>
#include "binheap.h"
#include "Room.h"
#include "Dungeon.h"

typedef struct pc {
    int x;
    int y;
    char standingOn;
} pc_t;

typedef enum colors{
    white,
    gray,
    black
} colors_t;

pc_t* createPlayerCharacter(room_t* rooms, int numRooms);

void drawPlayerCharacter(pc_t* pc);

void distanceToPlayerNoTunneling(pc_t* pc);

void distanceToPlayerTunneling(pc_t* pc);

#endif /* PlayerCharcter_h */
