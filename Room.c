//
//  Room.c
//  RogueLike
//
//  Created by Camden Voigt on 1/17/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include <stdio.h>
#include "Room.h"

void drawRoom(Room room) {
    int i = 0, j = 0;
    
    for(;i<room.height; i++) {
        for(;j<room.width; j++) {
            printf(".");
        }
        printf("\n");
    }
}