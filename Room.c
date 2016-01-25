//
//  Room.c
//  RogueLike
//
//  Created by Camden Voigt on 1/17/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "Room.h"

int findCenter(int num);

room_t getRandomRoom() {
    room_t room;
    
    room.width = rand() % 15 + 3;
    room.height = rand() % 8 + 2;
    
    room.x = rand() % (78 - room.width) + 1;
    room.y = rand() % (19 - room.height) + 1;
    
    room.centerX = findCenter((room.x + room.width) - room.x) + room.x;
    room.centerY = findCenter((room.y + room.height) - room.y) + room.y;
    
    return room;
}

int checkOverlappingRoom(room_t *room) {
    int i = room->y, j = room->x;
    int endX = j + room->width + 2, endY = i + room->height + 2;
    
    for(;i<endY;i++) {
        for(j = room->x;j<endX;j++) {
            if (dungeon[j - 1][i - 1].immutable == 1) {
                return 0;
            }
        }
    }
    
    return 1;
}

int findCenter(int num) {
    if (num%2 == 1) {
        return (num/2) +1;
    } else {
        return num/2;
    }
    
    return 0;
}