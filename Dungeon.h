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
#include <stdint.h>
#include <string.h>
#include <endian.h>
#include "Room.h"

typedef struct cell {
    char character;
    int immutable;
    int hardness;
} cell_t;

extern cell_t dungeon[80][21];

void drawDungeon();
void fillDungeon();
void createDungeon();
int addRoom();
void addCorridor();
void saveDungeon();
int loadDungeon();

#endif /* Dungeon_h */
