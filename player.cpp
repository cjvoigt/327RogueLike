//
//  PlayerCharcter.c
//  RogueLike
//
//  Created by Camden Voigt on 2/3/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "player.h"

#pragma mark - Prototypes

void swapPlayer(Player* player, int newX, int newY);
void checkMonsterDeath(int x, int y);
void initVisibleDungeon(Player* player);
void turnOffLights(Player* player); 

#pragma mark - Player Creation

player_t* createPlayer(room_t* rooms) {
    room_t room = rooms[0];

    int adjustment = 1;
    if(room.height > 2) {
        adjustment = 2;
    }

    Player* player = (Player*) new Player();
    player->x = room.x + (rand() % (room.width - 2)) + 1;
    player->y = room.y + (rand() % (room.height - adjustment)) + 1;
    player->dead = 0;
    player->speed = 10;
    dungeon[player->x][player->y].character = ((character_t* ) player);
    dungeon[player->x][player->y].type = '<';
    initVisibleDungeon(player);
    ((Character*) dungeon[player->x][player->y].character)->type = pc;

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

#pragma mark - Fog of War

cell_t emptyCell() {
    cell_t cell;
    cell.type = ' ';
    cell.hardness = 0;
    cell.immutable = 0;
    cell.visible = 0;
    cell.remembered = 0;
    cell.character = NULL;
    return cell;
}

void initVisibleDungeon(Player* player) {
    for(int i = 0; i < 21; i++) {
        for(int j = 0; j < 80; j++) {
            player->visibleDungeon[j][i] = emptyCell();
        }
    }
}

void turnOffLights(Player* player) {
    int startX = player->x - 3;
    if(startX < 0) {
        startX = 0;
    }

    int startY = player->y - 3;
    if(startY < 0) {
        startY = 0;
    }

    int endX = player->x + 3;
    if(endX > 80) {
        endX = 80;
    }

    int endY = player->y + 3;
    if(endY > 21) {
        endY = 21;
    }

    for(int i = startY; i < endY; i++) {
        for(int j = startX; j < endX; j++) {
            player->visibleDungeon[j][i].visible = 0;
        }
    }

}

void updateVisibleDungeon(player_t* p) {
    Player* player = (Player*)p;
    int startX = player->x - 3;
    if(startX < 0) {
        startX = 0;
    }

    int startY = player->y - 3;
    if(startY < 0) {
        startY = 0;
    }

    int endX = player->x + 3;
    if(endX > 80) {
        endX = 80;
    }

    int endY = player->y + 3;
    if(endY > 21) {
        endY = 21;
    }

    for(int i = startY; i < endY; i++) {
        for(int j = startX; j < endX; j++) {
            player->visibleDungeon[j][i] = dungeon[j][i];
            player->visibleDungeon[j][i].visible = 1;
            player->visibleDungeon[j][i].remembered = 1;
        }
    }
}

char getMonsterCharPlayer(int monsterID) {
    switch (monsterID) {
        case MONSTERA:
            return 'A';
        case MONSTERB:
            return 'B';
        case MONSTERC:
            return 'C';
        case MONSTERD:
            return 'D';
        case MONSTERE:
            return 'E';
        case MONSTERF:
            return 'F';
        default:
            return monsterID + '0';
    }
}

void drawVisibleDungeon(const char* topMessage, player_t* p) {
    Player* player = (Player*)p;
    int i, j;
    clear(); 
    mvprintw(0,0, "%s", topMessage);
    for(i = 0;i<21; i++) {
        for(j = 0;j<80; j++) {
            Character* character = (Character*) player->visibleDungeon[j][i].character;
            if(player->visibleDungeon[j][i].remembered == 0) {
                mvprintw(i + 1,j,"%c", ' ');
            } else if(player->visibleDungeon[j][i].visible == 0) {
                mvprintw(i + 1,j,"%c", player->visibleDungeon[j][i].type);
            } else if(character != NULL && character->type == pc) {
                mvprintw(i+ 1, j, "%c", '@');
            } else if(character != NULL && character->type == mon) {
                mvprintw(i + 1, j, "%c", getMonsterCharPlayer(getBehavior(((monster_t*)character))));
            } else  {
                mvprintw(i + 1, j, "%c", player->visibleDungeon[j][i].type);
            }
        }
    }
    refresh();
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

void swapPlayer(player_t* p, int newX, int newY) {
    Player* player = (Player*)p;
    if(dungeon[player->x + newX][player->y + newY].hardness != INFINITY) {
        checkMonsterDeath(player->x + newX, player->y + newY);
        if (dungeon[player->x + newX][player->y + newY].type == ' ') {
            dungeon[player->x + newX][player->y + newY].hardness = 0;
            dungeon[player->x + newX][player->y + newY].type = '#';
        }
        dungeon[player->x + newX][player->y + newY].character = ((character_t* ) player);
        ((Character*)dungeon[player->x][player->y].character)->type = pc;
        dungeon[player->x][player->y].character = NULL;
        turnOffLights(player);
        player->x += newX;
        player->y += newY;
        updateVisibleDungeon(p);
    }
}

void checkMonsterDeath(int x, int y) {
    if(dungeon[x][y].character != NULL) {
        if(((Character*)dungeon[x][y].character)->type == mon) {
            ((Character*)dungeon[x][y].character)->dead = 1;
            ((Character*)dungeon[x][y].character)->type = none;
        }
    }
}

void deletePlayer(player_t* p) {
    Player* player = (Player*)p;
    delete player;
}