//
//  Room.h
//  RogueLike
//
//  Created by Camden Voigt on 1/17/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#ifndef Room_h
#define Room_h

#include <stdio.h>
#include <stdlib.h>
#include "Dungeon.h"

typedef struct room {
    int x;
    int y;
    int width;
    int height;
    int centerX;
    int centerY;
} room_t;

room_t getRandomRoom();
int checkOverlappingRoom(room_t* room);

#endif /* Room_h */
