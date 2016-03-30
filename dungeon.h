//
//  Dungeon.h
//  RogueLike
//
//  Created by Camden Voigt on 1/18/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#ifndef Dungeon_h
#define Dungeon_h

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>

#include "character.h"

typedef struct cell {
    char type;
    int immutable;
    int hardness;
    int visible;
    int remembered;
    character_t* character;
} cell_t;

extern cell_t dungeon[80][21];

/* Full Dungeon */
void drawDungeon(const char* topMessage);
void fillDungeon();
void createDungeon(int numRooms, room_t* rooms);

/* Rooms */
int addRoom(room_t *room);
room_t getRandomRoom();
int checkOverlappingRoom(room_t* room);

/* Corridors */
void addCorridor(room_t *startRoom, room_t *endRoom);

/* File I/O */
void saveDungeon(int numRooms, room_t *rooms);
room_t* loadDungeon(int* numRooms);

#endif /* Dungeon_h */
