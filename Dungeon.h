//
//  Dungeon.h
//  RogueLike
//
//  Created by Camden Voigt on 1/18/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#ifndef Dungeon_h
#define Dungeon_h

#include <stdio.h>
#include "Room.h"

typedef struct cell {
    char character;
    int immutable;
} cell_t;

extern cell_t dungeon[80][21];

void drawDungeon();
void fillDungeon();
int addRoom();
void addCorridor();

#endif /* Dungeon_h */
