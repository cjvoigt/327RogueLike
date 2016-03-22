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
#include <unistd.h>
#include <limits.h>

#include "utils.h"
#include "Dungeon.h"
#include "Monster.h"

#pragma mark - Prototypes

room_t* setUp(int argc, char * argv[], int* numRooms, int*numMonsters);
room_t* handleArgs(int* array, int* numRooms, int* numMonsters);
character_t* setUpcharacters(int numMonsters, player_t* player, int numRooms, room_t* rooms);
int32_t compareCharacters(const void *key, const void *with);
void freeCharacter(void* key);
void takeAction(int direction, player_t* player, int* turn);

#pragma mark - Main

int main(int argc, char* argv[]) {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int numRooms, numMonsters;
    srand(time(NULL));

    room_t* rooms = setUp(argc, argv, &numRooms, &numMonsters);
    player_t* player = createPlayer(rooms);
    character_t* characters = setUpcharacters(numMonsters, player, numRooms, rooms);
    drawDungeon("Welcome to the Game");

    binheap_t pqueue;
    binheap_init_from_array(&pqueue, characters, sizeof(character_t), numMonsters + 1, compareCharacters, freeCharacter);

    while(numMonsters > 0 && getDead((character_t*)player) != 1) {
        character_t* character = (character_t*)binheap_remove_min(&pqueue);
        if (getDead(character) == 0) {
            if(getType(character) == player) {
                int turn = 0, ch = ' ';
                while(turn == 0) {
                    ch = getch();
                    char curChar = dungeon[getX(character)][getY(character)].type;
                    if((ch == 60 && curChar == '<') ||(ch == 62 && curChar == '>')) {
                        fillDungeon();
                        int temp[4] = {0, 0, 0, 0};
                        binheap_delete(&pqueue);
                        free(player);
                        free(characters);
                        free(rooms);
                        rooms = handleArgs(temp, &numRooms, &numMonsters);
                        player = malloc(sizeof(player_t));
                        player = createPlayer(rooms);
                        characters = setUpcharacters(numMonsters, player, numRooms, rooms);
                        binheap_init_from_array(&pqueue, characters, sizeof(character_t), numMonsters + 1, compareCharacters, freeCharacter);
                        drawDungeon("");
                        break;
                    } else if (ch == '<' || ch == '>') {
                        continue;
                    } else if (ch == 'm') {
                         drawMonsterList(characters, numMonsters);
                    } else if (ch == 'S') {
                        saveDungeon(numRooms, rooms);
                        break; 
                    } else {
                        takeAction(ch, player, &turn);
                    }
                }
                if(ch == '<' || ch == '>') {
                    continue;
                } else if (ch == 'S') {
                    break;
                }
                findLineOfSightMultiple(characters, numMonsters + 1, player, rooms, numRooms);
            } else if (getType(character) == mon) {
                moveMonster((monster_t*)character, player);
                findLineOfSightSingle(character, numMonsters + 1, player, rooms, numRooms);
           }
            setTurn(character, getTurn(character) + 100/getSpeed(character));
            binheap_insert(&pqueue, character);
            drawDungeon("");
        } else if (getDead(character) == 1) {
            numMonsters--;
            setTurn(character, INT_MAX);
            binheap_insert(&pqueue, character);
        }
    }

    if(getDead((character_t*)player) == 1) {
        drawDungeon("You Died! Press e to exit");
    } else if(numMonsters == 0) {
        drawDungeon("You Won! Press e to exit");
    } else {
        drawDungeon("Press e to exit");
    }

    binheap_delete(&pqueue);
    free(characters);
    free(rooms);

    int end = 0;
    while (end != 'e') {
        end = getch();
    }

    endwin();
    return 0;
}

#pragma mark - Set Up

room_t* setUp(int argc, char* argv[], int* numRooms, int* numMonsters) {
    int i;
    int args[4] = {0,0,0,0};
    fillDungeon();
    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--load") == 0) {
            args[0] = TRUE;
        } else if(strcmp(argv[i], "--save") == 0) {
            args[1] = TRUE;
        } else if(strcmp(argv[i], "--nummon") == 0) {
            args[2] = TRUE;
            if(i + 1 < argc && strcmp(argv[i + 1], "--save") != 0 && strcmp(argv[i + 1], "--load") != 0) {
                args[3] = atoi(argv[i + 1]);
                i++;
            }
        } else {
            printf("Tag not recgonized %s", argv[i]);
        }
    }
    return handleArgs(args, numRooms, numMonsters);
}

room_t* handleArgs(int* array, int* numRooms, int* numMonsters) {
    room_t* rooms = NULL;

    //handles loading and saving
    if(array[0] != 0 || array[1] != 0) {
        //checks for loading
        if(array[0] == 1) {
            rooms = loadDungeon(numRooms);
        }
        //checks for saving
        if(array[1] == 1 && array[0] == 0) {
            *numRooms = rand() % 5 + 6;
            rooms = malloc(*numRooms * sizeof(room_t));
            createDungeon(*numRooms, rooms);
            saveDungeon(*numRooms, rooms);
        } else if (array[1] == 1 && array[0] == 1) {
            if(rooms != NULL) {
                saveDungeon(*numRooms, rooms);
            }
        }
    } else {
        //no load or save
        *numRooms = rand() % 5 + 6;
        room_t* temp = malloc(*numRooms * sizeof(room_t));
        if(temp != NULL) {
            rooms = temp;
        }
        createDungeon(*numRooms, rooms);
    }

    //handles --nummon
    if(array[2] == 1) {
        *numMonsters = array[3];
    } else if (array[2] == 0) {
        *numMonsters = rand() % 10 + 1;
    }

    return rooms;
}

character_t* setUpcharacters(int numMonsters, player_t* player, int numRooms, room_t* rooms) {
    character_t* characters = malloc(sizeof(character_t) * (numMonsters+1));;
    for(int i = 1; i < numMonsters + 1; i++) {
        monster_t* monster = createMonster(rooms, numRooms);
        characters[i] = *((character_t*)monster);
    }

    return characters;
}

#pragma mark - Priority Queue

int32_t compareCharacters(const void *key, const void *with) {
    character_t* character1 = (character_t*) key;
    character_t* character2 = (character_t*) with;

    if(getTurn(character1) - getTurn(character2) != 0) {
        return getTurn(character1) - getTurn(character2);
    } else {
        return getPriority(character1) - getPriority(character2);
    }
}

void freeCharacter(void* key) {
    character_t* character = (character_t*) key;
    free(character);
}

void takeAction(int direction, player_t* player, int* turn) {
    *turn = 1;
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
        case 32:
            break;
         default:
            *turn = 0;
            break;
    }
}