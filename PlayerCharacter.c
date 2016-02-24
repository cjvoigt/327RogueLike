//
//  PlayerCharcter.c
//  RogueLike
//
//  Created by Camden Voigt on 2/3/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "PlayerCharacter.h"

#pragma mark - Prototypes

void swapPlayer(pc_t* pc, int newX, int newY);
void checkMonsterDeath(int x, int y);

#pragma mark - Player Creation

pc_t* createPlayerCharacter(room_t* rooms) {
    room_t room = rooms[0];
    
    int adjustment = 1;
    if(room.height > 2) {
        adjustment = 2;
    }
    
    pc_t* pc = malloc(sizeof(pc_t));
    pc->x = room.x + (rand() % (room.width - 2)) + 1;
    pc->y = room.y + (rand() % (room.height - adjustment)) + 1;
    pc->dead = 0;
    dungeon[pc->x][pc->y].character = playerAsCharacter(pc);
    
    return pc;
}

character_t playerAsCharacter(pc_t* pc) {
    character_t character;
    character.type = player;
    character.speed = 10;
    character.sequence = 0;
    character.turn = 0;
    character.charID.player = pc;
    return character;
}

#pragma mark - Player Movement

void movePlayer(pc_t* pc) {
    int direction = rand() % 8;
    switch (direction) {
        case 0:
            swapPlayer(pc, -1, -1);
            break;
        case 1:
            swapPlayer(pc, 0, -1);
            break;
        case 2:
            swapPlayer(pc, 1, -1);
            break;
        case 3:
            swapPlayer(pc, -1, 0);
            break;
        case 4:
            swapPlayer(pc, 1, 0);
            break;
        case 5:
            swapPlayer(pc, -1, 1);
            break;
        case 6:
            swapPlayer(pc, 0, 1);
            break;
        case 7:
            swapPlayer(pc, 1, 1);
            break;
    }
}

void swapPlayer(pc_t* pc, int newX, int newY) {
    if(dungeon[pc->x + newX][pc->y + newY].hardness != INFINITY) {
        checkMonsterDeath(pc->x + newX, pc->y + newY);
        if (dungeon[pc->x + newX][pc->y + newY].type == ' ') {
            dungeon[pc->x + newX][pc->y + newY].hardness = 0;
            dungeon[pc->x + newX][pc->y + newY].type = '#';
        }
        dungeon[pc->x + newX][pc->y + newY].character.charID.player = pc;
        dungeon[pc->x + newX][pc->y + newY].character.type = player;
        dungeon[pc->x][pc->y].character.charID.player = NULL;
        dungeon[pc->x][pc->y].character.type = none;
        pc->x += newX;
        pc->y += newY;
    }
}

void checkMonsterDeath(int x, int y) {
    if(dungeon[x][y].character.type == mon) {
        dungeon[x][y].character.charID.monster->dead = 1;
        dungeon[x][y].character.type = none;
    }
}