//
//  PlayerCharcter.c
//  RogueLike
//
//  Created by Camden Voigt on 2/3/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "player.h"

#pragma mark - Prototypes

void swapPlayer(player_t* player, int newX, int newY);
void checkMonsterDeath(int x, int y);

#pragma mark - Player Creation

player_t* createPlayer(room_t* rooms) {
    room_t room = rooms[0];
    
    int adjustment = 1;
    if(room.height > 2) {
        adjustment = 2;
    }
    
    Player* player = (Player*) malloc(sizeof(Player));
    player->x = room.x + (rand() % (room.width - 2)) + 1;
    player->y = room.y + (rand() % (room.height - adjustment)) + 1;
    player->dead = 0;
    player->speed = 10;
    dungeon[player->x][player->y].character = ((character_t* ) player);
    dungeon[player->x][player->y].type = '<';
    
    return (player_t*) player;
}

void changePos(player_t* player, room_t* rooms) {
    room_t room = rooms[0];

    int adjustment = 1;
    if(room.height > 2) {
        adjustment = 2;
    }

    ((Player *) player)->x = room.x + (rand() % (room.width - 2)) + 1;
    ((Player *) player)->y = room.y + (rand() % (room.height - adjustment)) + 1;
}

#pragma mark - Player Movement

void movePlayer(player_t* player, int direction) {
    switch (direction) {
        case 55:
            swapPlayer(player, -1, -1);
            break;
        case 121:
            swapPlayer(player, -1, -1);
            break;
        case 56:
            swapPlayer(player, 0, -1);
            break;
        case 107:
            swapPlayer(player, 0, -1);
            break;
        case 57:
            swapPlayer(player, 1, -1);
            break;
        case 117:
            swapPlayer(player, 1, -1);
            break;
        case 52:
            swapPlayer(player, -1, 0);
            break;
        case 104:
            swapPlayer(player, -1, 0);
            break;
        case 54:
            swapPlayer(player, 1, 0);
            break;
        case 108:
            swapPlayer(player, 1, 0);
            break;
        case 49:
            swapPlayer(player, -1, 1);
            break;
        case 98:
            swapPlayer(player, -1, 1);
            break;
        case 50:
            swapPlayer(player, 0, 1);
            break;
        case 106:
            swapPlayer(player, 0, 1);
            break;
        case 51:
            swapPlayer(player, 1, 1);
            break;
        case 110:
            swapPlayer(player, 1, 1);
            break;
        default:
            printw("%d", direction);
            break;
    }
}

void swapPlayer(player_t* player, int newX, int newY) {
    if(dungeon[((Player *) player)->x + newX][((Player *) player)->y + newY].hardness != INFINITY) {
        checkMonsterDeath(((Player *) player)->x + newX, ((Player *) player)->y + newY);
        if (dungeon[((Player *) player)->x + newX][((Player *) player)->y + newY].type == ' ') {
            dungeon[((Player *) player)->x + newX][((Player *) player)->y + newY].hardness = 0;
            dungeon[((Player *) player)->x + newX][((Player *) player)->y + newY].type = '#';
        }
        dungeon[((Player *) player)->x + newX][((Player *) player)->y + newY].character = ((character_t* ) player);
        dungeon[((Player *) player)->x + newX][((Player *) player)->y + newY].character.type = player;
        dungeon[((Player *) player)->x][((Player *) player)->y].character = NULL;
        dungeon[((Player *) player)->x][((Player *) player)->y].character.type = none;
        ((Player *) player)->x += newX;
        ((Player *) player)->y += newY;
    }
}

void checkMonsterDeath(int x, int y) {
    if(dungeon[x][y].character.type == mon) {
        dungeon[x][y].character.charID.monster->dead = 1;
        dungeon[x][y].character.type = none;
    }
}