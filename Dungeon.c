//
//  Dungeon.c
//  RogueLike
//
//  Created by Camden Voigt on 1/18/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "Dungeon.h"

cell_t dungeon[80][21];

#pragma mark - Prototypes

void getPoint(room_t *startRoom, room_t *nextRoom, int *x, int *y);
int goHorizontal(int startX, int endX, int *y);
void goVertical(int startY, int endY, int x);
int turn();
char* getFilePath();
void redrawDungeon();

#pragma mark - Full Dungeon

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
            dungeon[j][i].hardness = rand() % 254 + 1;
            if (i == 0 || i == 20 || j == 0 || j == 79){
                dungeon[j][i].immutable = 1;
            } else {
                dungeon[j][i].immutable = 0;
            }
        }
    }
}

void createDungeon(int numRooms, room_t* rooms) {
    for(int i = 0; i < numRooms; i++) {
        rooms[i] = getRandomRoom();
        while(checkOverlappingRoom(&rooms[i]) == 0) {
            rooms[i] =  getRandomRoom();
        }
        addRoom(rooms[i]);
    }
    
    for(int j = 0; j < numRooms-1; j++){
        addCorridor(&rooms[j], &rooms[j+1]);
    }
    drawDungeon();
    printf("\n\n\n");
}

#pragma mark - Rooms

int addRoom(room_t *room) {
    int i = room->y, j = room->x, endX = j + room->width, endY = i + room->height;
    
    for(;i<endY;i++) {
        for(j = room->x;j<endX;j++) {
            dungeon[j][i].character = '.';
            dungeon[j][i].immutable = 1;
            dungeon[j][i].hardness = 0;
        }
    }
    
    return 0;
}

#pragma mark - Corridors

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
                dungeon[i][*y].hardness = 0;
            }
            int newY = turn();
            
            if(newY != 0 ) {
                *y += newY;
                if(dungeon[i][*y].immutable != 1) {
                    dungeon[i][*y].character = '#';
                    dungeon[i][*y].hardness = 0;
                }
            }
        }
    } else if (startX >= endX) {
        for(i = startX; i > endX; i--) {
            if(dungeon[i][*y].immutable != 1) {
                dungeon[i][*y].character = '#';
                dungeon[i][*y].hardness = 0;
            }
            int newY = turn();
            
            if(newY != 0 ) {
                *y += newY;
                if(dungeon[i][*y].immutable != 1) {
                    dungeon[i][*y].character = '#';
                    dungeon[i][*y].hardness = 0;
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
                dungeon[x][i].hardness = 0;
            }
        }
    } else if (startY < endY) {
        for(i = startY; i <= endY; i++) {
            if(dungeon[x][i].immutable != 1) {
                dungeon[x][i].character ='#';
                dungeon[x][i].hardness = 0;
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

#pragma mark - File I/O

void saveDungeon(int numRooms, room_t *rooms) {
    //init variables
    char fileMarker[] = "RLG327";
    uint32_t version = 0, beversion = 0;
    uint32_t size = 1496 + (numRooms * 4), besize = 0;

    char* fullPath = getFilePath();
    FILE* fp = fopen(fullPath , "wb+");
    free(fullPath);
    
    //write file marker
    fwrite(fileMarker, 1, sizeof(fileMarker) - 1, fp);
    
    //write version number
    beversion = htobe32(version);
    fwrite(&beversion, sizeof(version), 1, fp);
    
    //write size
    besize = htobe32(size);
    fwrite(&besize, sizeof(size), 1, fp);
    
    //write dungeon
    int i, j;
    for(i = 1; i < 20; i++) {
        for(j = 1; j < 79; j++) {
            fwrite(&dungeon[j][i].hardness, 1, 1, fp);
        }
    }
    
    //write rooms
    int k;
    for(k = 0; k < numRooms; k++) {
        uint8_t x = rooms[k].x, y = rooms[k].y, width = rooms[k].width, height = rooms[k].height;
        fwrite(&x, 1, 1, fp);
        fwrite(&y, 1, 1, fp);
        fwrite(&width, 1, 1, fp);
        fwrite(&height, 1, 1, fp);
    }
    
    //close file
    fclose(fp);
}

void loadDungeon(int* numRooms, room_t* rooms) {
    //open file
    char* fullPath = getFilePath();
    FILE* fp = fopen(fullPath , "rb+");
    free(fullPath);
    
    //Get FileMarker
    char fileMarker[7];
    fread(fileMarker, 1, sizeof(fileMarker) - 1, fp);
    
    //get version and size
    uint32_t beversion, besize;
    uint32_t version, size;
    fread(&beversion, 4, 1, fp);
    fread(&besize, 4, 1, fp);
    version = be32toh( beversion);
    size = be32toh(besize);
    
    //get dungeon
    int i, j;
    for(i = 1; i < 20; i++) {
        for(j = 1; j < 79; j++) {
            fread(&dungeon[j][i].hardness, 1, 1, fp);
        }
    }
    
    //get rooms
    int k;
    *numRooms = (size - 1496)/4;
    for(k = 0; k < *numRooms; k++) {
        uint8_t buffer[4];
        fread(buffer, 1, 4, fp);
        room_t room;
        room.x = buffer[0];
        room.y = buffer[1];
        room.width = buffer[2];
        room.height = buffer[3];
        addRoom(&room);
        rooms[k] = room;
    }
    
    redrawDungeon();
    
    fclose(fp);
}

char* getFilePath() {
    char* homePath = getenv("HOME");
    char* filePath = "/.rlg327/dungeon.rlg327";
    long length = strlen(homePath) + strlen(filePath) + 1;
    char* fullPath = malloc(length);
    snprintf(fullPath, length, "%s%s", homePath, filePath);
    return fullPath;
}

void redrawDungeon() {
    int i, j;
    for(i = 0; i < 21; i++) {
        for(j = 0; j < 80; j++) {
            if(dungeon[j][i].hardness == 0 && dungeon[j][i].immutable == 0) {
                dungeon[j][i].character = '#';
            }
        }
    }
}