//
//  Monster.c
//  RogueLike
//
//  Created by Camden Voigt on 2/16/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "monster.h"

#pragma mark - Data Structures

typedef enum colors{
    white,
    gray,
    black
} colors_t;

#pragma mark - Prototypes

void swapMonster(Monster* monster, int newX, int newY, int tunneling);
void randomMove(Monster* monster, int tunneling);
void straightMove(Monster* monster, int tunneling);
void shortestMove(Monster* monster, int tunneling);
int sameRoom(Monster* monster, Player* player, room_t* rooms, int numRooms);
int straightLine(Monster* monster, Player* player);
int checkLine(int x, int endX, int y, int endY, int dx, int dy);
void checkForKill(int x, int y);
void distanceToPlayerNoTunneling(Player* player, int results[][21]);
void distanceToPlayerTunneling(Player* player, int results[][21]);
int32_t comparePosition(const void *key, const void *with);
int** getDistanceDungeon(position_t* distanceDungeon[][21]);
void printDistance(position_t* array[][21]);
void deleteDistanceDungeon(position_t* positions[][21]);
int adjustHardness(int current);

#pragma mark - Monster Printing

char getMonsterChar(int monsterID) {
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

void drawMonsterList(character_t* characters, int numMonsters) {
    int i,  button, start = 1; 
    Player* player = (Player*)&characters[0];
    do {
        clear();
        printw( "Monster List\n");
        Monster* currentMonster = ((Monster*)&characters[i]);
        for(i = start; i <= numMonsters && i <= start + 21; i++) {
            printw( "%d. %c, ",i, getMonsterChar(currentMonster->behavior));
            if(player->y - currentMonster->behavior > 0) {
                printw("%d North and ", player->y - currentMonster->y);
            } else {
                printw( "%d South and ", (player->y - currentMonster->y) * -1);
            }

            if(player->x - currentMonster->x > 0) {
                printw( "%d West.", player->x - currentMonster->x);
            } else {
                printw("%d East.", (player->x - currentMonster->x) * -1);
            }
            printw("\n");
        }
        refresh();
        button = getch();
        if(button == KEY_UP && start > 1) {
            start--;
        } else if(button == KEY_DOWN && (numMonsters - start) > 21) {
            start++;
        }
    } while(button != 27);
}

#pragma mark - Monster Creation:

monster_t* createMonster(room_t* rooms, int numRooms) {
    int randRoomNumber = (rand() % (numRooms - 1)) + 1;
    room_t room = rooms[randRoomNumber];

    Monster* monster = new Monster();
    monster->x = room.x + rand() % room.width;
    monster->y = room.y + rand() % room.height;
    while(dungeon[monster->x][monster->y].character != NULL) {
        monster->x = room.x + rand() % room.width;
        monster->y = room.y + rand() % room.height;
    }
    monster->behavior = rand() % 16;
    monster->lastX = INFINITY;
    monster->lastY = INFINITY;
    monster->dead = 0;
    monster->visible = 0;
    dungeon[monster->x][monster->y].character = ((character_t*) monster);
    ((Character*)dungeon[monster->x][monster->y].character)->type = mon;
    return (monster_t*)monster;
}

#pragma mark - Monster Movement

void moveMonster(monster_t* m, player_t* p) {
    int m1 = 0x1;
    int m2 = 0x3;
    int m3 = 0x7;
    int m4 = 0xF;

    Player* player = (Player*)p;
    Monster* monster = (Monster*)m;
    
    int intelligent = monster->behavior & m1;
    int telepathy = (monster->behavior & m2) - intelligent;
    int tunneling = (monster->behavior & m3) - telepathy - intelligent;
    int erratic = (monster->behavior & m4) - tunneling - telepathy - intelligent;

    telepathy /= 2;
    tunneling /= 4;
    erratic /= 8;

    if(telepathy == 1) {
        monster->lastX = player->x;
        monster->lastY = player->y;
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

void randomMove(Monster* monster, int tunneling) {
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

void straightMove(Monster* monster, int tunneling) {
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

void shortestMove(Monster* monster, int tunneling) {
    if(monster->lastX != INFINITY && (monster->lastX != monster->x || monster->lastY != monster->y)) {

        Player* tempplayer = (Player*)malloc(sizeof(Player));
        int distanceDungeon[80][21];

        tempplayer->x = monster->lastX;
        tempplayer->y = monster->lastY;
        tempplayer->dead = 0;

        if (tunneling == 0) {
            distanceToPlayerNoTunneling(tempplayer, distanceDungeon);
        } else {
            distanceToPlayerTunneling(tempplayer, distanceDungeon);
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
        free(tempplayer);
    }
}

void swapMonster(Monster* monster, int newX, int newY, int tunneling) {
    checkForKill(newX, newY);
    if(tunneling == 1) {
        if (dungeon[newX][newY].type == ' ') {
            dungeon[newX][newY].hardness = 0;
            dungeon[newX][newY].type = '#';
        }
        dungeon[newX][newY].character = (character_t*)monster;
        ((Character*)dungeon[newX][newY].character)->type = mon;
        dungeon[monster->x][monster->y].character = NULL;
        ((Character*)dungeon[monster->x][monster->y].character)->type = none;
        monster->x = newX;
        monster->y = newY;
    } else {
        if(dungeon[newX][newY].type != ' ') {
            dungeon[newX][newY].character = (character_t*)monster;
            ((Character*)dungeon[newX][newY].character)->type = mon;
            dungeon[monster->x][monster->y].character = NULL;
            ((Character*)dungeon[monster->x][monster->y].character)->type = none;
            monster->x = newX;
            monster->y = newY;
        }
    }
}

void checkForKill(int x, int y) {
    Character* currentCharacter = (Character*)dungeon[x][y].character;
    if(currentCharacter->type == mon) {
        currentCharacter->dead = 1;
        currentCharacter->type = none;
    } else if(currentCharacter->type == pc) {
        currentCharacter->dead = 1;
        currentCharacter->type = none;
    }
}

#pragma mark - Line of Sight

void findLineOfSightMultiple(character_t* characters, int numChars, player_t* p, room_t* rooms, int numRooms) {
    Player* player = (Player*)p;
    for(int i = 1; i < numChars; i++) {
        Monster* monst = (Monster*)&characters[i];
        if(sameRoom(monst, player, rooms, numRooms) == TRUE) {
            monst->visible = 1;
            monst->lastX = player->x;
            monst->lastY = player->y;
        } else if(straightLine(monst, player) == TRUE) {
            monst->visible = 1;
            monst->lastX = player->x;
            monst->lastY = player->y;
        } else {
            monst->visible = 0;
        }
    }
}

void findLineOfSightSingle(character_t* character, int numChars, player_t* p, room_t* rooms, int numRooms) {
    Player* player = (Player*)p;
    Monster* monst = (Monster*)character;
    if(sameRoom(monst, player, rooms, numRooms) == TRUE) {
        monst->visible = 1;
        monst->lastX = player->x;
        monst->lastY = player->y;
    } else if(straightLine(monst, player) == TRUE) {
        monst->visible = 1;
        monst->lastX = player->x;
        monst->lastY = player->y;
    } else {
        monst->visible = 0;
    }
}

int sameRoom(Monster* monster, Player* player, room_t* rooms, int numRooms) {
    int i;
    
    //Finds which room the player is in
    for(i = 0; i < numRooms; i++) {
        if(player->x >= rooms[i].x && player->x <= (rooms[i].x + rooms[i].width)) {
            if(player->y >= rooms[i].y && player->y <= (rooms[i].y + rooms[i].height)) {
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

int straightLine(Monster* monster, Player* player) {
    if(player->x == monster->x) {
        if(player->y - monster->y > 0) {
            return checkLine(monster->x, player->x, monster->y, player->y, 0, -1);
        } else {
            return checkLine(monster->x, player->x, monster->y, player->y, 0, 1);
        }
    } else if(player->y == monster->y) {
        if(player->x - monster->x > 0) {
            return checkLine(monster->x, player->x, monster->y, player->y, 1, 0);
        } else {
            return checkLine(monster->x, player->x, monster->y, player->y, -1, 0);
        }
    } else if(player->x - monster->x == player->y - monster->y) {
        if(player->y - monster->y > 0 && player->x - monster->x > 0) {
            return checkLine(monster->x, player->x, monster->y, player->y, 1, -1);
        } else if (player->y - monster->y > 0 && player->x - monster->x < 0) {
            return checkLine(monster->x, player->x, monster->y, player->y, -1 , -1);
        } else if (player->y - monster->y < 0 && player->x - monster->x > 0) {
            return checkLine(monster->x, player->x, monster->y, player->y, -1 , 1);
        } else {
            return checkLine(monster->x, player->x, monster->y, player->y, 1 , 1);
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

void distanceToPlayerNoTunneling(Player* player, int results[][21]) {
    int i, j;
    position_t* distanceDungeon[80][21];
    colors_t colors[80][21];
    binheap_t heap;
    binheap_init(&heap, comparePosition, free);

    for(i = 0; i < 21; i++) {
        for(j = 0; j < 80; j++) {
            if(dungeon[j][i].hardness == 0) {
                distanceDungeon[j][i] = (position_t*)malloc(sizeof(position_t));
                distanceDungeon[j][i]->distance = INFINITY;
                distanceDungeon[j][i]->x = j;
                distanceDungeon[j][i]->y = i;
                results[j][i] = INFINITY;
                colors[j][i] = white;
            } else {
                distanceDungeon[j][i] = (position_t*)malloc(sizeof(position_t));
                distanceDungeon[j][i]->distance = INFINITY;
                distanceDungeon[j][i]->x = j;
                distanceDungeon[j][i]->y = i;
                results[j][i] = INFINITY;
                colors[j][i] = black;
            }
        }
    }
    
    distanceDungeon[player->x][player->y]->distance = 0;
    results[player->x][player->y] = 0;
    colors[player->x][player->y] = gray;
    binheap_insert(&heap, distanceDungeon[player->x][player->y]);
    
    while(binheap_is_empty(&heap) == 0) {
        position_t* curPos = (position_t*) binheap_remove_min(&heap);
        if(curPos == NULL) { printf("Error"); }
        colors[curPos->x][curPos->y] = black;
        
        for(i = curPos->x - 1; i <= curPos->x + 1; i++) {
            for(j = curPos->y - 1; j <= curPos->y + 1; j++) {
                if(colors[i][j] == white) {
                    distanceDungeon[i][j]->distance = curPos->distance + 1;;
                    results[i][j] = curPos->distance + 1;
                    binheap_insert(&heap, distanceDungeon[i][j]);
                    colors[i][j] = gray;
                }
            }
        }
    }
	deleteDistanceDungeon(distanceDungeon);
	binheap_delete(&heap);
}

void distanceToPlayerTunneling(Player* player, int results[][21]) {
    int i, j;
    position_t* distanceDungeon[80][21];
    binheap_t heap;
    binheap_init(&heap, comparePosition, NULL);
    
    for(i = 0; i < 21; i++) {
        for(j = 0; j < 80; j++) {
            if(i != player->y || j != player->x) {
                distanceDungeon[j][i] = (position_t*)malloc(sizeof(position_t));
                distanceDungeon[j][i]->x = j;
                distanceDungeon[j][i]->y = i;
                distanceDungeon[j][i]->distance = INFINITY;
                results[j][i] = INFINITY;
                distanceDungeon[j][i]->heapNode = binheap_insert(&heap, distanceDungeon[j][i]);
            } else {
                distanceDungeon[j][i] = (position_t*)malloc(sizeof(position_t));
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
	deleteDistanceDungeon(distanceDungeon);
	binheap_delete(&heap);
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
            free(positions[j][i]);;
        }
    }
}

#pragma mark - Gettters

int getBehavior(const monster_t* monster){
    return ((Monster*)monster)->behavior;
}

int getLastX(const monster_t* monster) {
    return ((Monster*)monster)->lastX;
}

int getLastY(const monster_t* monster) {
    return ((Monster*)monster)->lastY;
}

int getVisible(const monster_t* monster) {
    return ((Monster*)monster)->visible;
}

#pragma mark - Setters

void setBehavior(monster_t* monster, int behavior) {
    ((Monster*)monster)->behavior = behavior;
}

void setLastX(monster_t* monster, int lastX) {
    ((Monster*)monster)->lastX = lastX;
}

void setLastY(monster_t* monster, int lastY) {
    ((Monster*)monster)->lastY = lastY;
}

void setVisible(monster_t* monster, int visible) {
    ((Monster*)monster)->visible = visible;
}