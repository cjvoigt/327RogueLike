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

void createDungeon();

int main(int argc, const char * argv[]) {
    // set Seed
    int t;
    //srand(1453501536);
    srand(t = time(NULL));
    printf("seed: %d",t);
    
    //createDungeon
    createDungeon();
    
    return 0;
}

void createDungeon() {
    fillDungeon();
    int numRooms = rand() % 5 + 6;
    room_t rooms[numRooms];
    for(int i = 0; i < numRooms; i++) {
        rooms[i] = getRandomRoom();
        while(checkOverlappingRoom(&rooms[i]) == 0) {
            rooms[i] =  getRandomRoom();
        }
        addRoom(rooms[i]);
    }
    
    for(int j = 0; j < numRooms-1; j++){
        addCorridor(&rooms[j], &rooms[j+1]);
    }
    drawDungeon();
    printf("\n\n\n");
}
