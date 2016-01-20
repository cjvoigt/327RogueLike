//
//  main.c
//  RogueLike
//
//  Created by Camden Voigt on 1/17/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Room.h"
#include "Dungeon.h"
#include "Constants.h"

int main(int argc, const char * argv[]) {
    srand(time(NULL));
    fillDungeon();
    Room rooms[5];
    for(int i = 0; i < 5; i++) {
        rooms[i] = getRandomRoom();
        while(checkOverlappingRoom() == 0) {
            rooms[i] =  getRandomRoom();
        }
        addRoom(rooms[i]);
    }
    drawDungeon(); 
    printf("\n\n\n");
    return 0;
}
