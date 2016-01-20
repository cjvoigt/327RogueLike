//
//  Room.c
//  RogueLike
//
//  Created by Camden Voigt on 1/17/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "Room.h"

void drawRoom(Room room) {
    int i = 0, j = 0;
    
    for(;i<room.height; i++) {
        for(;j<room.width; j++) {
            printf(".");
        }
        j = 0;
        printf("\n");
    }
}

Room getRandomRoom() {
    Room room;
    
    room.x = rand() % 75;
    room.y = rand() % 16;
    room.width = rand() % 15 + 2;
    room.height = rand() % 5 + 2;
    
    return room;
}

bool checkOverlappingRoom(Room room) {
    int i = room.y, j = room.x, endX = j + room.width, endY = i + room.height;
    
    for(;i<endY;i++) {
        for(j = room.x;j<endX;j++) {
            if (dungeon[j][i] == '.') {
                return false;
            }
        }
    }
    
    return true;
}