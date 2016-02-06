//
//  PlayerCharcter.c
//  RogueLike
//
//  Created by Camden Voigt on 2/3/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "PlayerCharcter.h"

typedef struct position {
    int x;
    int y;
    int distance;
    binheap_node_t* heapNode;
} position_t;

int32_t comparePosition(const void *key, const void *with);
void printDistance(position_t* array[][21]);
void deleteDistanceDungeon(position_t* positions[][21]);
int adjustHardness(int current);

pc_t* createPlayerCharacter(room_t* rooms, int numRooms) {
    int roomIndex = rand() % numRooms;
    room_t room = rooms[roomIndex];
    
    int adjustment = 1;
    if(room.height > 2) {
        adjustment = 2;
    }
    
    pc_t* pc = malloc(sizeof(pc_t));
    pc->x = room.x + (rand() % (room.width - 2)) + 1;
    pc->y = room.y + (rand() % (room.height - adjustment)) + 1;
    
    return pc;
}

void drawPlayerCharacter(pc_t* pc) {
    pc->standingOn = dungeon[pc->x][pc->y].character;
    dungeon[pc->x][pc->y].character = '@';
    drawDungeon();
}

void distanceToPlayerNoTunneling(pc_t* pc) {
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
                colors[j][i] = white;
            } else {
                distanceDungeon[j][i] = malloc(sizeof(position_t));
                distanceDungeon[j][i]->distance = INFINITY;
                colors[j][i] = black;
            }
        }
    }
    
    distanceDungeon[pc->x][pc->y]->distance = 0;
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
                    binheap_insert(&heap, next);
                    colors[i][j] = gray;
                }
            }
        }
    }
    printDistance(distanceDungeon);
}

void distanceToPlayerTunneling(pc_t* pc) {
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
                distanceDungeon[j][i]->heapNode = binheap_insert(&heap, distanceDungeon[j][i]);
            } else {
                distanceDungeon[j][i] = malloc(sizeof(position_t));
                distanceDungeon[j][i]->x = j;
                distanceDungeon[j][i]->y = i;
                distanceDungeon[j][i]->distance = 0;
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
                        binheap_decrease_key(&heap, next->heapNode);
                    }
                }
            }
        }
    }
    printDistance(distanceDungeon);
    deleteDistanceDungeon(distanceDungeon);
}

void fillDistanceDungeon(int** distanceDungeon, pc_t* pc) {
    int i, j;
    
    for(i = 0; i < 21; i++) {
        for(j = 0; j < 80; j++) {
            distanceDungeon[j][i] = INFINITY;
        }
    }
    
    distanceDungeon[pc->x][pc->y] = 0;
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
                printf("%c", dungeon[j][i].character);
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