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
#include "PlayerCharacter.h"
#include "Monster.h"

#pragma mark - Prototypes

room_t* setUp(int argc, char * argv[], int* numRooms, int*numMonsters);
room_t* handleArgs(int* array, int* numRooms, int* numMonsters);
character_t* setUpPlayers(int numMonsters, pc_t* pc, int numRooms, room_t* rooms);
void setUpNewDungeon(int* numRooms, room_t* rooms, int* numMonsters, character_t* players, pc_t* pc, binheap_t* pqueue);
int32_t compareCharacters(const void *key, const void *with);
void freeCharacter(void* key);
void takeAction(int direction, pc_t* pc, character_t* monsters, int numMonsters);


#pragma mark - Main

int main(int argc, char* argv[]) {
    initscr();
    noecho();
    curs_set(0);

    int numRooms, numMonsters;
    srand(time(NULL));

    room_t* rooms = setUp(argc, argv, &numRooms, &numMonsters);
    pc_t* pc = createPlayerCharacter(rooms);
    character_t* players = setUpPlayers(numMonsters, pc, numRooms, rooms);
    drawDungeon("Welcome to the Game");

    binheap_t pqueue;
    binheap_init_from_array(&pqueue, players, sizeof(character_t), numMonsters + 1, compareCharacters, freeCharacter);

    while(numMonsters > 0 && pc->dead != 1) {
        character_t* character = (character_t*)binheap_remove_min(&pqueue);
        if (character->charID.player->dead == 0 || character->charID.monster->dead == 0) {
            if(character->type == player) {
                int ch = getch();
                if(ch == 60 || ch == 62) {
                    fillDungeon();
                    int temp[4] = {0, 0, 0, 0};
                    binheap_delete(&pqueue);
                    free(pc);
                    free(players);
                    free(rooms);
                    rooms = handleArgs(temp, &numRooms, &numMonsters);
                    pc = malloc(sizeof(pc_t));
                    pc = createPlayerCharacter(rooms);
                    players = setUpPlayers(numMonsters, pc, numRooms, rooms);
                    binheap_init_from_array(&pqueue, players, sizeof(character_t), numMonsters + 1, compareCharacters, freeCharacter);
                    drawDungeon("");
                    continue;
                } else {
                    takeAction(ch, pc, players, numMonsters);
                }
                findLineOfSightMultiple(players, numMonsters + 1, pc, rooms, numRooms);
            } else if (character->type == mon) {
                moveMonster(character->charID.monster, pc);
                findLineOfSightSingle(character, numMonsters + 1, pc, rooms, numRooms);
           }
            character->turn += (100/character->speed);
            binheap_insert(&pqueue, character);
            drawDungeon("");
        } else if (character->charID.monster->dead == 1) {
            numMonsters--;
            character->turn = INT_MAX;
            binheap_insert(&pqueue, character);
        }
    }

    if(pc->dead == 1) {
        drawDungeon("You Died!");
    } else {
        drawDungeon("You Won!");
    }

    binheap_delete(&pqueue);
    free(players);
    free(rooms);

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
        rooms = malloc(*numRooms * sizeof(room_t));
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

character_t* setUpPlayers(int numMonsters, pc_t* pc, int numRooms, room_t* rooms) {
    character_t* players = malloc(sizeof(character_t) * (numMonsters+1));
    players[0] = playerAsCharacter(pc);
    for(int i = 1; i < numMonsters + 1; i++) {
        monster_t* monster = createMonster(rooms, numRooms);
        players[i] = monsterAsCharacter(monster, i);
    }

    return players;
}

void setUpNewDungeon(int* numRooms, room_t* rooms, int* numMonsters, character_t* players, pc_t* pc, binheap_t* pqueue) {
    fillDungeon();
    int temp[4] = {0, 0, 0, 0};
    binheap_delete(pqueue);
    free(pc);
    free(players);
    free(rooms);
    rooms = handleArgs(temp, numRooms, numMonsters);
    pc = malloc(sizeof(pc_t));
    pc = createPlayerCharacter(rooms);
    players = setUpPlayers(*numMonsters, pc, *numRooms, rooms);
    binheap_init_from_array(pqueue, players, sizeof(character_t), *numMonsters + 1, compareCharacters, freeCharacter);
    drawDungeon("");
}

#pragma mark - Priority Queue

int32_t compareCharacters(const void *key, const void *with) {
    character_t* character1 = (character_t*) key;
    character_t* character2 = (character_t*) with;

    if(character1->turn - character2->turn != 0) {
        return character1->turn - character2->turn;
    } else {
        return character1->sequence - character2->sequence;
    }
}

void freeCharacter(void* key) {
    character_t* character = (character_t*) key;
    if(character->type == mon) {
        free(character->charID.monster);
    } else if (character->type == player) {
        free(character->charID.player);
    }
}

void takeAction(int direction, pc_t* pc, character_t* monsters, int numMonsters) {
    switch (direction) {
         case 55:
            swapPlayer(pc, -1, -1);
            break;
        case 121:
            swapPlayer(pc, -1, -1);
            break;
        case 56:
            swapPlayer(pc, 0, -1);
            break;
        case 107:
            swapPlayer(pc, 0, -1);
            break;
        case 57:
            swapPlayer(pc, 1, -1);
            break;
        case 117:
            swapPlayer(pc, 1, -1);
            break;
        case 52:
            swapPlayer(pc, -1, 0);
            break;
        case 104:
            swapPlayer(pc, -1, 0);
            break;
        case 54:
            swapPlayer(pc, 1, 0);
            break;
        case 108:
            swapPlayer(pc, 1, 0);
            break;
        case 49:
            swapPlayer(pc, -1, 1);
            break;
        case 98:
            swapPlayer(pc, -1, 1);
            break;
        case 50:
            swapPlayer(pc, 0, 1);
            break;
        case 106:
            swapPlayer(pc, 0, 1);
            break;
        case 51:
            swapPlayer(pc, 1, 1);
            break;
        case 110:
            swapPlayer(pc, 1, 1);
            break;
        case 109:
            drawMonsterList(monsters, numMonsters);
            int button = getch();
            while(button != 27) {
                continue;
            }
            break; 
         defualt:
            drawDungeon("Can't Press this");
            break;
    }
}