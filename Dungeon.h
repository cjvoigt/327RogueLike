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
#include "Constants.h"

extern char dungeon[80][21];

void drawDungeon();
void fillDungeon();
bool addRoom();

#endif /* Dungeon_h */
