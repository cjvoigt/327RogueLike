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
#include "Constants.h"

typedef struct Rooms {
    int x;
    int y;
    int width;
    int height;
} Room;

void drawRoom();
Room getRandomRoom();
bool checkOverlappingRoom();

#endif /* Room_h */
