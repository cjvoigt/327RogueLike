//
//  Dungeon.c
//  RogueLike
//
//  Created by Camden Voigt on 1/18/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "Dungeon.h"

char dungeon[80][21];

void drawDungeon() {
    int i = 0, j = 0;
    
    for(;i<21; i++) {
        for(j = 0;j<80; j++) {
            printf("%c", dungeon[j][i]);
        }
        printf("\n");
    }
}

void fillDungeon() {
    int i, j;
    
    for(i = 0;i<21; i++) {
        for(j = 0;j<80; j++) {
            dungeon[j][i] = ' ';
        }
    }
}

bool addRoom(Room room) {
    int i = room.y, j = room.x, endX = j + room.width, endY = i + room.height;
    
    for(;i<endY;i++) {
        for(j = room.x;j<endX;j++) {
            dungeon[j][i] = '.';
        }
    }
    
    return true;
}