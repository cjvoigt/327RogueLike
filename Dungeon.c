//
//  Dungeon.c
//  RogueLike
//
//  Created by Camden Voigt on 1/18/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "Dungeon.h"

cell_t dungeon[80][21];

void getPoint(room_t *startRoom, room_t *nextRoom, int *x, int *y);
int goHorizontal(int startX, int endX, int *y);
void goVertical(int startY, int endY, int x);
int turn();

void drawDungeon() {
    int i = 0, j = 0;
    
    for(;i<21; i++) {
        for(j = 0;j<80; j++) {
            printf("%c", dungeon[j][i].character);
        }
        printf("\n");
    }
}

void fillDungeon() {
    int i, j;
    
    for(i = 0;i<21; i++) {
        for(j = 0;j<80; j++) {
            dungeon[j][i].character = ' ';
            if (i == 0 || i == 20 || j == 0 || j == 79){
                dungeon[j][i].immutable = 1;
            } else {
                dungeon[j][i].immutable = 0;
            }
        }
    }
}

int addRoom(room_t *room) {
    int i = room->y, j = room->x, endX = j + room->width, endY = i + room->height;
    
    for(;i<endY;i++) {
        for(j = room->x;j<endX;j++) {
            dungeon[j][i].character = '.';
            dungeon[j][i].immutable = 1;
        }
    }
    
    return 0;
}

void addCorridor(room_t *startRoom, room_t *endRoom) {
    int startX, startY;
    int endX, endY;
    
    getPoint(startRoom, endRoom, &startX, &startY);
    getPoint(endRoom, startRoom, &endX, &endY);
    
    int x = goHorizontal(startX, endX, &startY);
    goVertical(startY, endY, x);
}

void getPoint(room_t *startRoom, room_t *nextRoom, int *x, int *y) {
    int disX = abs(startRoom->centerX - nextRoom->centerX);
    int disY = abs(startRoom->centerY - nextRoom->centerY);
    
    if(disX < disY){
        if(startRoom->centerX - nextRoom->centerX < 0) {
            *y = rand() % startRoom->height + startRoom->y;
            *x = startRoom->x - 1;
        } else {
            *y = rand() % startRoom->height + startRoom->y;
            *x = startRoom->x + startRoom->width;
        }
    } else if(disX > disY) {
        if(startRoom->centerY - nextRoom->centerY < 0){
            *x = rand() % startRoom->width + startRoom->x;
            *y = startRoom->y + startRoom->height;
        } else {
            *x = rand() % startRoom->width + startRoom->x;
            *y = startRoom->y - 1;
        }
    } else if(disX == disY) {
        if(startRoom->centerX - nextRoom->centerX < 0) {
            *y = rand() % startRoom->height + startRoom->y;
            *x = startRoom->x - 1;
        } else {
            *y = rand() % startRoom->height + startRoom->y;
            *x = startRoom->x + startRoom->width;
        }
    }
}

int goHorizontal(int startX, int endX, int *y) {
    int i = 0;
    
    if (startX < endX) {
        for(i = startX; i <= endX; i++){
            if(dungeon[i][*y].immutable != 1) {
                dungeon[i][*y].character = '#';
            }
            int newY = turn();
            
            if(newY != 0 ) {
                *y += newY;
                if(dungeon[i][*y].immutable != 1) {
                    dungeon[i][*y].character = '#';
                }
            }
        }
    } else if (startX >= endX) {
        for(i = startX; i > endX; i--) {
            if(dungeon[i][*y].immutable != 1) {
                dungeon[i][*y].character = '#';
            }
            int newY = turn();
            
            if(newY != 0 ) {
                *y += newY;
                if(dungeon[i][*y].immutable != 1) {
                    dungeon[i][*y].character = '#';
                }
            }
        }
    } else {
        return startX;
    }
    
    return i;
}

void goVertical(int startY, int endY, int x) {
    int i = 0;
    
    if (startY > endY) {
        for(i = startY; i >= endY; i--) {
            if(dungeon[x][i].immutable != 1) {
                dungeon[x][i].character ='#';
            }
        }
    } else if (startY < endY) {
        for(i = startY; i <= endY; i++) {
            if(dungeon[x][i].immutable != 1) {
                dungeon[x][i].character ='#';
            }
        }
    } else {
        return;
    }
}

int turn(){
    int num = rand() % 20;
    if (num == 2) {
        return 1;
    } else if (num == 3) {
        return -1;
    } else {
        return 0;
    }
}
