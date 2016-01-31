//
//  main.c
//  RogueLike
//
//  Created by Camden Voigt on 1/17/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Room.h"
#include "Dungeon.h"

void setUp();

int main(int argc, const char * argv[]) {
    // set Seed
    //srand(1453501536);
    srand(time(NULL));
    fillDungeon();
    setUp(argc, argv);
    
    return 0;
}

void setUp(int argc, char * argv[]) {
    if(argc == 2) {
        if(strcmp(argv[1], "--load") == 0) {
            int numRooms = 0;
            room_t rooms[15];
            loadDungeon(&numRooms, rooms);
            drawDungeon();
        } else if (strcmp(argv[1], "--save") == 0) {
            int numRooms = rand() % 5 + 6;
            room_t rooms[numRooms];
            createDungeon(numRooms, rooms);
            saveDungeon(numRooms, rooms);
        } else {
            printf("Error: Don't understand tag %s", argv[1]);
        }
    } else if(argc == 3) {
        if(strcmp(argv[1], "--load") == 0 && strcmp(argv[2], "--save") == 0) {
            int numRooms = 0;
            room_t rooms[15];
            loadDungeon(&numRooms, rooms);
            drawDungeon();
            saveDungeon(numRooms, rooms);
        } else if (strcmp(argv[1], "--save") == 0 && strcmp(argv[2], "--load") == 0) {
            int numRooms = 0;
            room_t rooms[15];
            loadDungeon(&numRooms, rooms);
            drawDungeon();
            saveDungeon(numRooms, rooms);
        } else {
            printf("Error: Don't understand tags %s and %s", argv[1], argv[2]);
        }
    } else if(argc > 3) {
        printf("Error: Too many arguments");
    } else  {
        int numRooms = rand() % 5 + 6;
        room_t rooms[numRooms];
        createDungeon(numRooms, &rooms);
    }
}