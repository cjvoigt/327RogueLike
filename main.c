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
#include <curses.h>
#include "Room.h"
#include "Dungeon.h"
#include "PlayerCharcter.h"

room_t* setUp(int argc, char * argv[], int* numRooms);

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    int numRooms;
    room_t* rooms = setUp(argc, argv, &numRooms);
    pc_t* pc = createPlayerCharacter(rooms, numRooms);
    drawPlayerCharacter(pc);
    distanceToPlayerNoTunneling(pc);
    distanceToPlayerTunneling(pc);
    
    free(pc);
    free(rooms);
    return 0;
}

room_t* setUp(int argc, char* argv[], int* numRooms) {
    fillDungeon();
    room_t* rooms;
    if(argc == 2) {
        if(strcmp(argv[1], "--load") == 0) {
            rooms = loadDungeon(numRooms);
        } else if (strcmp(argv[1], "--save") == 0) {
            *numRooms = rand() % 5 + 6;
            rooms = malloc(*numRooms * sizeof(room_t));
            createDungeon(*numRooms, rooms);
            saveDungeon(*numRooms, rooms);
        } else {
            printf("Error: Don't understand tag %s", argv[1]);
        }
    } else if(argc == 3) {
        if(strcmp(argv[1], "--load") == 0 && strcmp(argv[2], "--save") == 0) {
            rooms = loadDungeon(numRooms);
            if(rooms != NULL) {
                saveDungeon(*numRooms, rooms);
                free(rooms);
            }
        } else if (strcmp(argv[1], "--save") == 0 && strcmp(argv[2], "--load") == 0) {
            rooms = loadDungeon(numRooms);
            if(rooms != NULL) {
                saveDungeon(*numRooms, rooms);
                free(rooms);
            }
        } else {
            printf("Error: Don't understand tags %s and %s", argv[1], argv[2]);
        }
    } else if(argc > 3) {
        printf("Error: Too many arguments");
    } else  {
        *numRooms = rand() % 5 + 6;
        rooms = malloc(*numRooms * sizeof(room_t));
        createDungeon(*numRooms, rooms);
    }

    return rooms;
}