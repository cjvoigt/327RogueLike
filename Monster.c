//
//  Monster.c
//  RogueLike
//
//  Created by Camden Voigt on 2/16/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "Monster.h"

#pragma mark - Data Structures

typedef enum colors{
    white,
    gray,
    black
} colors_t;

#pragma mark - Prototypes

void swapMonster(monster_t* monster, int newX, int newY, int tunneling);
void randomMove(monster_t* monster, int tunneling);
void straightMove(monster_t* monster, int tunneling);
void shortestMove(monster_t* monster, int tunneling);
int sameRoom(monster_t* monster, pc_t* pc, room_t* rooms, int numRooms);
int straightLine(monster_t* monster, pc_t* pc);
int checkLine(int x, int endX, int y, int endY, int dx, int dy);
void checkForKill(int x, int y);
int32_t comparePosition(const void *key, const void *with);
int** getDistanceDungeon(position_t* distanceDungeon[][21]);
void printDistance(position_t* array[][21]);
void deleteDistanceDungeon(position_t* positions[][21]);
int adjustHardness(int current);

#pragma mark - Monster Creation

monster_t* createMonster(room_t* rooms, int numRooms) {
    int randRoomNumber = (rand() % (numRooms - 1)) + 1;
    room_t room = rooms[randRoomNumber];
    
    monster_t* monster = malloc(sizeof(monster_t));
    monster->x = room.x + rand() % room.width;
    monster->y = room.y + rand() % room.height;
    while(dungeon[monster->x][monster->y].character.type == player || dungeon[monster->x][monster->y].character.type == mon) {
        monster->x = room.x + rand() % room.width;
        monster->y = room.y + rand() % room.height;
    }
    monster->behavior = rand() % 16;
    monster->lastX = INFINITY;
    monster->lastY = INFINITY;
    monster->dead = 0;
    monster->visible = 0;
    dungeon[monster->x][monster->y].character.charID.monster = monster;
    dungeon[monster->x][monster->y].character.type = mon;
    return monster;
}

character_t* monsterAsCharacter(monster_t* monster, int num) {
    character_t* character = malloc(sizeof(character_t));
    character->type = mon;
    character->sequence = num;
    character->speed = (rand() % 16) + 5;
    character->turn = 0;
    character->charID.monster = monster;
    return  character;
}

#pragma mark - Monster Movement

void moveMonster(monster_t* monster, pc_t* pc) {
    int m1 = 0x1;
    int m2 = 0x3;
    int m3 = 0x7;
    int m4 = 0xF;
    
    int intelligent = monster->behavior & m1;
    int telepathy = (monster->behavior & m2) - intelligent;
    int tunneling = (monster->behavior & m3) - telepathy - intelligent;
    int erratic = (monster->behavior & m4) - tunneling - telepathy - intelligent;
    
    telepathy /= 2;
    tunneling /= 4;
    erratic /= 8;
    
    if(telepathy == 1) {
        monster->lastX = pc->x;
        monster->lastY = pc->y;
        monster->visible = 1;
    }
    
    if (erratic == 1 && (rand() % 2) == 1) {
        randomMove(monster, tunneling);
    } else if (intelligent == 0) {
        straightMove(monster, tunneling);
    } else if (intelligent == 1){
        shortestMove(monster, tunneling);
    }
}

void randomMove(monster_t* monster, int tunneling) {
    int direction = rand() % 8;
    switch (direction) {
        case 0:
            if(tunneling == 1){
                swapMonster(monster, monster->x - 1, monster->y - 1, 1);
            } else {
                swapMonster(monster, monster->x - 1, monster->y - 1, 0);
            }
            break;
        case 1:
            if(tunneling == 1){
                swapMonster(monster, monster->x, monster->y - 1, 1);
            } else {
                swapMonster(monster, monster->x, monster->y - 1, 0);
            }
            break;
        case 2:
            if(tunneling == 1){
                swapMonster(monster, monster->x + 1, monster->y - 1, 1);
            } else {
                swapMonster(monster, monster->x + 1, monster->y - 1, 0);
            }
            break;
        case 3:
            if(tunneling == 1){
                swapMonster(monster, monster->x - 1, monster->y, 1);
            } else {
                swapMonster(monster, monster->x - 1, monster->y, 0);
            }
            break;
        case 4:
            if(tunneling == 1){
                swapMonster(monster, monster->x + 1, monster->y, 1);
            } else {
                swapMonster(monster, monster->x + 1, monster->y, 0);
            }
            break;
        case 5:
            if(tunneling == 1){
                swapMonster(monster, monster->x - 1, monster->y + 1, 1);
            } else {
                swapMonster(monster, monster->x - 1, monster->y + 1, 0);
            }
            break;
        case 6:
            if(tunneling == 1){
                swapMonster(monster, monster->x, monster->y + 1, 1);
            } else {
                swapMonster(monster, monster->x, monster->y + 1, 0);
            }
            break;
        case 7:
            if(tunneling == 1){
                swapMonster(monster, monster->x + 1, monster->y + 1, 1);
            } else {
                swapMonster(monster, monster->x + 1, monster->y + 1, 0);
            }
            break;
    }
}

void straightMove(monster_t* monster, int tunneling) {
    if(monster->visible != 1) {
        return;
    }
    
    if (monster->x < monster->lastX && (dungeon[monster->x + 1][monster->y].type != ' ' || tunneling == 1)) {
        swapMonster(monster, monster->x + 1, monster->y, tunneling);
    } else if (monster->x > monster->lastX && (dungeon[monster->x - 1][monster->y].type != ' ' || tunneling == 1)) {
        swapMonster(monster, monster->x - 1, monster->y, tunneling);
    } else if (monster->y < monster->lastY) {
        swapMonster(monster, monster->x, monster->y + 1, tunneling);
    } else if (monster->y > monster->lastY) {
        swapMonster(monster, monster->x, monster->y - 1, tunneling);
    }
}

void shortestMove(monster_t* monster, int tunneling) {
    if(monster->lastX != INFINITY && (monster->lastX != monster->x || monster->lastY != monster->y)) {
        
        pc_t* tempPC = malloc(sizeof(pc_t));
        int distanceDungeon[80][21];
        
        tempPC->x = monster->lastX;
        tempPC->y = monster->lastY;
        tempPC->dead = 0;
        
        if (tunneling == 0) {
            distanceToPlayerNoTunneling(tempPC, distanceDungeon);
        } else {
            distanceToPlayerTunneling(tempPC, distanceDungeon);
        }
        
        int least = INFINITY;
        int x = monster->x, y = monster->y;
        for(int i = monster->y - 1; i <= monster->y + 1; i++) {
            for(int j = monster->x - 1; j <= monster->x + 1; j++) {
                if(distanceDungeon[j][i] < least) {
                    least = distanceDungeon[j][i];
                    x = j;
                    y = i;
                }
            }
        }
        
        swapMonster(monster, x, y, tunneling);
        free(tempPC);
    }
}

void swapMonster(monster_t* monster, int newX, int newY, int tunneling) {
    checkForKill(newX, newY);
    if(tunneling == 1) {
        if (dungeon[newX][newY].type == ' ') {
            dungeon[newX][newY].hardness = 0;
            dungeon[newX][newY].type = '#';
        }
        dungeon[newX][newY].character.charID.monster = monster;
        dungeon[newX][newY].character.type = mon;
        dungeon[monster->x][monster->y].character.charID.monster = NULL;
        dungeon[monster->x][monster->y].character.type = none;
        monster->x = newX;
        monster->y = newY;
    } else {
        if(dungeon[newX][newY].type != ' ') {
            dungeon[newX][newY].character.charID.monster = monster;
            dungeon[newX][newY].character.type = mon;
            dungeon[monster->x][monster->y].character.charID.monster = NULL;
            dungeon[monster->x][monster->y].character.type = none;
            monster->x = newX;
            monster->y = newY;
        }
    }
}

void checkForKill(int x, int y) {
    if(dungeon[x][y].character.type == mon) {
        dungeon[x][y].character.charID.monster->dead = 1;
        dungeon[x][y].character.type = none;
    } else if(dungeon[x][y].character.type == player) {
        dungeon[x][y].character.charID.player->dead = 1;
        dungeon[x][y].character.type = none;
    }
}

#pragma mark - Line of Sight

void findLineOfSightMultiple(character_t* characters, int numChars, pc_t* pc, room_t* rooms, int numRooms) {
    for(int i = 1; i < numChars; i++) {
        monster_t* monst = characters[i].charID.monster;
        if(sameRoom(monst, pc, rooms, numRooms) == TRUE) {
            monst->visible = 1;
            monst->lastX = pc->x;
            monst->lastY = pc->y;
        } else if(straightLine(monst, pc) == TRUE) {
            monst->visible = 1;
            monst->lastX = pc->x;
            monst->lastY = pc->y;
        } else {
            monst->visible = 0;
        }
    }
}

void findLineOfSightSingle(character_t* character, int numChars, pc_t* pc, room_t* rooms, int numRooms) {
    monster_t* monst = character->charID.monster;
    if(sameRoom(monst, pc, rooms, numRooms) == TRUE) {
        monst->visible = 1;
        monst->lastX = pc->x;
        monst->lastY = pc->y;
    } else if(straightLine(monst, pc) == TRUE) {
        monst->visible = 1;
        monst->lastX = pc->x;
        monst->lastY = pc->y;
    } else {
        monst->visible = 0;
    }
}

int sameRoom(monster_t* monster, pc_t* pc, room_t* rooms, int numRooms) {
    int i;
    
    //Finds which room the pc is in
    for(i = 0; i < numRooms; i++) {
        if(pc->x >= rooms[i].x && pc->x <= (rooms[i].x + rooms[i].width)) {
            if(pc->y >= rooms[i].y && pc->y <= (rooms[i].y + rooms[i].height)) {
                break;
            }
        }
    }
    
    //Checks if monster is in that room
    if(monster->x >= rooms[i].x && monster->x <= (rooms[i].x + rooms[i].width)) {
        if(monster->y >= rooms[i].y && monster->y <= (rooms[i].y + rooms[i].height)) {
            return TRUE;
        }
    }
    
    return FALSE;
}

int straightLine(monster_t* monster, pc_t* pc) {
    if(pc->x == monster->x) {
        if(pc->y - monster->y > 0) {
            return checkLine(monster->x, pc->x, monster->y, pc->y, 0, -1);
        } else {
            return checkLine(monster->x, pc->x, monster->y, pc->y, 0, 1);
        }
    } else if(pc->y == monster->y) {
        if(pc->x - monster->x > 0) {
            return checkLine(monster->x, pc->x, monster->y, pc->y, 1, 0);
        } else {
            return checkLine(monster->x, pc->x, monster->y, pc->y, -1, 0);
        }
    } else if(pc->x - monster->x == pc->y - monster->y) {
        if(pc->y - monster->y > 0 && pc->x - monster->x > 0) {
            return checkLine(monster->x, pc->x, monster->y, pc->y, 1, -1);
        } else if (pc->y - monster->y > 0 && pc->x - monster->x < 0) {
            return checkLine(monster->x, pc->x, monster->y, pc->y, -1 , -1);
        } else if (pc->y - monster->y < 0 && pc->x - monster->x > 0) {
            return checkLine(monster->x, pc->x, monster->y, pc->y, -1 , 1);
        } else {
            return checkLine(monster->x, pc->x, monster->y, pc->y, 1 , 1);
        }
    } else {
        return FALSE;
    }
}

int checkLine(int x, int endX, int y, int endY, int dx, int dy) {
    while (x == endX || y == endY ) {
        x += dx;
        y += dy;
        if (dungeon[x][y].hardness != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

#pragma mark - Distance to Player

void distanceToPlayerNoTunneling(pc_t* pc, int results[][21]) {
    int i, j;
    position_t* distanceDungeon[80][21];
    colors_t colors[80][21];
    binheap_t heap;
    binheap_init(&heap, comparePosition, free);
    
    for(i = 0; i < 21; i++) {
        for(j = 0; j < 80; j++) {
            if(dungeon[j][i].hardness == 0) {
                distanceDungeon[j][i] = malloc(sizeof(position_t));
                distanceDungeon[j][i]->distance = INFINITY;
                results[j][i] = INFINITY;
                colors[j][i] = white;
            } else {
                distanceDungeon[j][i] = malloc(sizeof(position_t));
                distanceDungeon[j][i]->distance = INFINITY;
                results[j][i] = INFINITY;
                colors[j][i] = black;
            }
        }
    }
    
    distanceDungeon[pc->x][pc->y]->distance = 0;
    results[pc->x][pc->y] = 0;
    position_t* p;
    p = malloc(sizeof(position_t));
    p->distance = 0;
    p->x = pc->x;
    p->y = pc->y;
    colors[pc->x][pc->y] = gray;
    binheap_insert(&heap, p);
    
    while(binheap_is_empty(&heap) == 0) {
        position_t* curPos = (position_t*) binheap_remove_min(&heap);
        if(curPos == NULL) { printf("Error"); }
        colors[curPos->x][curPos->y] = black;
        
        for(i = curPos->x - 1; i <= curPos->x + 1; i++) {
            for(j = curPos->y - 1; j <= curPos->y + 1; j++) {
                if(colors[i][j] == white) {
                    position_t* next;
                    next = malloc(sizeof(position_t));
                    next->x = i;
                    next->y = j;
                    next->distance = curPos->distance + 1;
                    distanceDungeon[i][j]->distance = next->distance;
                    results[i][j] = next->distance;
                    binheap_insert(&heap, next);
                    colors[i][j] = gray;
                }
            }
        }
    }
}

void distanceToPlayerTunneling(pc_t* pc, int results[][21]) {
    int i, j;
    position_t* distanceDungeon[80][21];
    binheap_t heap;
    binheap_init(&heap, comparePosition, NULL);
    
    for(i = 0; i < 21; i++) {
        for(j = 0; j < 80; j++) {
            if(i != pc->y || j != pc->x) {
                distanceDungeon[j][i] = malloc(sizeof(position_t));
                distanceDungeon[j][i]->x = j;
                distanceDungeon[j][i]->y = i;
                distanceDungeon[j][i]->distance = INFINITY;
                results[j][i] = INFINITY;
                distanceDungeon[j][i]->heapNode = binheap_insert(&heap, distanceDungeon[j][i]);
            } else {
                distanceDungeon[j][i] = malloc(sizeof(position_t));
                distanceDungeon[j][i]->x = j;
                distanceDungeon[j][i]->y = i;
                distanceDungeon[j][i]->distance = 0;
                results[j][i] = 0;
                distanceDungeon[j][i]->heapNode = binheap_insert(&heap, distanceDungeon[j][i]);
            }
        }
    }
    
    while(binheap_is_empty(&heap) == 0) {
        position_t* curPos = (position_t*) binheap_remove_min(&heap);
        if(curPos == NULL) { printf("Error"); }
        
        for(i = curPos->x - 1; i <= curPos->x + 1; i++) {
            for(j = curPos->y - 1; j <= curPos->y + 1; j++) {
                if(i > 0 && i < 80 && j > 0 && j < 21) {
                    position_t* next = distanceDungeon[i][j];
                    if(next->distance > curPos->distance + adjustHardness(dungeon[i][j].hardness)){
                        next->distance = curPos->distance + adjustHardness(dungeon[i][j].hardness);
                        results[i][j] = curPos->distance + adjustHardness(dungeon[i][j].hardness);
                        binheap_decrease_key(&heap, next->heapNode);
                    }
                }
            }
        }
    }
}

void printDistance(position_t* array[][21]) {
    int i, j;
    for(i = 0; i < 21; i++) {
        for(j = 0; j < 80; j++) {
            if(array[j][i]->distance < 10) {
                printf("%c", array[j][i]->distance + 48);
            } else if(array[j][i]->distance < 36) {
                printf("%c", array[j][i]->distance + 87);
            } else if(array[j][i]->distance < 62) {
                printf("%c", array[j][i]->distance + 29);
            } else {
                printf("%c", dungeon[j][i].type);
            }
        }
        printf("\n");
    }
}

int adjustHardness(int current) {
    if(current < 85) {
        return 1;
    } else if (current < 171) {
        return 2;
    } else {
        return 3;
    }
}

int32_t comparePosition(const void *key, const void *with) {
    position_t* a = (position_t*)key;
    position_t* b = (position_t*)with;
    return a->distance - b->distance;
}

void deleteDistanceDungeon(position_t* positions[][21]) {
    int i, j;
    for(i = 0; i < 21; i++){
        for(j = 0; j < 80; j++) {
            position_t* p = positions[j][i];
            free(p);
        }
    }
}