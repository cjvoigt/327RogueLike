//
//  main.c
//  RogueLike
//
//  Created by Camden Voigt on 1/17/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "monsterparser.h"
#include "monster.h"

#pragma mark - Prototypes

room_t* setUp(int argc, char * argv[], int* numRooms, int*numMonsters);
room_t* handleArgs(int* array, int* numRooms, int* numMonsters);
character_t** setUpcharacters(int numMonsters, player_t* player, int numRooms, room_t* rooms);
int32_t compareCharacters(const void *key, const void *with);
void freeCharacter(void* key);
void takeAction(int direction, player_t* player, int* turn);
bool checkColor(string color); 

#pragma mark - Main

int main(int argc, char* argv[]) {
    string filepath = getenv("HOME");
    filepath += "/.rlg327/monster_desc.txt";
    ifstream f(filepath.c_str());
    string currentLine, currentWord;
    vector<monsterparser> monsters;
    string params[8];
    bool flags[9], failed;

    if(!f.good()) {
        cout << "File did not open" << endl;
        return -1;
    }

    getline(f, currentLine);
    if(currentLine != "RLG327 MONSTER DESCRIPTION 1"){
        return -1;
    }

    failed = false;
    while(getline(f, currentLine)) {
        stringstream ss;
        if(failed == true) {
            while(getline(f, currentLine)) {
                if(currentLine == "BEGIN MONSTER") {
                    failed = false;
                    break;
                 }
            }
        }

        if (currentLine == "") {
            continue;
        }

        ss << currentLine;
        ss >> currentWord;
        if(currentLine == "BEGIN MONSTER") {
           for(int i = 0; i < 9; i++) {
                if(i < 8) {
                     params[i] = "";
                }
                flags[i] = false;
           }
           flags[8] = true;
        } else if(currentLine == "END") {
            for(int i = 0; i < 9; i++) {
                if(flags[i] != true) {
                    failed = true;
                    break;
                }
            }
            if(failed) { continue; }
            monsterparser monster(params);
            monsters.push_back(monster);
        } else if(currentWord == "NAME") {
            if(flags[0] == true) {
                failed = true;
                continue;
            }
            string name;
            while(ss >> currentWord) {
                name += currentWord + " ";
            }
            params[0] = name;
            flags[0] = true;
        } else if(currentWord == "SYMB") {
            if(flags[1] == true) {
                failed = true;
                continue;
            }
            ss >> currentWord;
            params[1] = currentWord;
            flags[1] = true;
        } else if(currentWord == "DESC") {
             if(flags[2] == true) {
                failed = true;
                continue;
            }

            currentWord = "";
            getline(f, currentLine);
            currentWord += currentLine;
            getline(f, currentLine);
            while(currentLine != "." && currentLine != "BEGIN MONSTER") {
                if(currentLine != "" && currentLine.at(0) != ' ') {
                    currentWord += " " + currentLine;
                } else {
                    currentWord += currentLine;
                }
                getline(f, currentLine);
            }

            if(currentLine == "BEGIN MONSTER") {
                for(int i = 0; i < 8; i++) {
                    params[i] = "";
                    flags[i] = false;
                }
                flags[9] = true;
                continue;
            }

            params[2] = currentWord;
            flags[2] = true;
        } else if(currentWord == "COLOR") {
            if(flags[3] == true) {
                failed = true;
                continue; 
            }
            ss >> currentWord;
            if(!checkColor(currentWord)) { 
                failed = true;
                continue;
            }
            params[3] = currentWord;
            flags[3] = true;
        } else if(currentWord == "SPEED") {
            if(flags[4] == true) {
                failed = true;
                continue; 
            }
            ss >> currentWord;
            params[4] = currentWord;
            flags[4] = true;
        } else if(currentWord == "ABIL") {
            if(flags[5] == true) {
                failed = true;
                continue; 
            }
            currentWord = currentLine.substr(5);
            params[5] = currentWord;
            flags[5] = true;
        } else if(currentWord == "HP") {
            if(flags[6] == true) {
                failed = true;
                continue; 
            }
            ss >> currentWord;
            params[6] = currentWord;
            flags[6] = true;
        } else if(currentWord == "DAM") {
            if(flags[7] == true) {
                failed = true;
                continue; 
            }
            ss >> currentWord;
            params[7] = currentWord;
            flags[7] = true;
        } else {
            failed = true;
        }
    }

    vector<monsterparser>::iterator i;
    for (i = monsters.begin(); i != monsters.end(); i++) {
        (*i).print();
        cout << "\n";
    }

    return 0;

    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int numRooms, numMonsters;
    srand(time(NULL));

    room_t* rooms = setUp(argc, argv, &numRooms, &numMonsters);
    player_t* player = createPlayer(rooms);
    character_t** characters = setUpcharacters(numMonsters, player, numRooms, rooms);
    updateVisibleDungeon(player);
    drawVisibleDungeon("Welcome to the Game", player);

    binheap_t pqueue;
    binheap_init_from_array(&pqueue, characters, 8, numMonsters + 1, compareCharacters, freeCharacter);

    while(numMonsters > 0 && getDead((character_t*)player) != 1) {
        character_t** character = (character_t**)binheap_remove_min(&pqueue);
        if (getDead(*character) == 0) {
            if(getType(*character) == pc) {
                int turn = 0, ch = ' ';
                while(turn == 0) {
                    ch = getch();
                    char curChar = dungeon[getX(*character)][getY(*character)].type;
                    if((ch == 60 && curChar == '<') ||(ch == 62 && curChar == '>')) {
                        fillDungeon();
                        int temp[4] = {0, 0, 0, 0};
                        binheap_insert(&pqueue, character);
                        binheap_delete(&pqueue);
                        deleteCharacterArray(characters);
                        free(rooms);
                        rooms = handleArgs(temp, &numRooms, &numMonsters);
                        player = createPlayer(rooms);
                        characters = setUpcharacters(numMonsters, player, numRooms, rooms);
                        binheap_init_from_array(&pqueue, characters, 8, numMonsters + 1, compareCharacters, freeCharacter);
                        updateVisibleDungeon(player);
                        drawVisibleDungeon("", player);
                        break;
                    } else if (ch == '<' || ch == '>') {
                        continue;
                    } else if (ch == 'm') {
                        drawMonsterList(characters, numMonsters);
                        drawVisibleDungeon("", player);
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
            } else if (getType(*character) == mon) {
                moveMonster((monster_t*)*character, player);
                updateVisibleDungeon(player);
                findLineOfSightSingle(*character, numMonsters + 1, player, rooms, numRooms);
           }
            setTurn(*character, getTurn(*character) + 100/getSpeed(*character));
            binheap_insert(&pqueue, character);
            drawVisibleDungeon("", player);
        } else if (getDead(*character) == 1) {
            numMonsters--;
            setTurn(*character, INT_MAX);
            binheap_insert(&pqueue, character);
        }
   }

    if(getDead((character_t*)player) == 1) {
        drawVisibleDungeon("You Died! Press e to exit", player);
    } else if(numMonsters == 0) {
        drawVisibleDungeon("You Won! Press e to exit", player);
    } else {
        drawVisibleDungeon("Press e to exit", player);
    }

    binheap_delete(&pqueue);
    deleteCharacterArray(characters);
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
            rooms = (room_t *)malloc(*numRooms * sizeof(room_t));
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
        room_t* temp =(room_t *) malloc(*numRooms * sizeof(room_t));
        if(temp != NULL) {
            rooms = temp;
            createDungeon(*numRooms, rooms);
        } 
   }

    //handles --nummon
    if(array[2] == 1) {
        *numMonsters = array[3];
    } else if (array[2] == 0) {
        *numMonsters = rand() % 10 + 1;
    }

    return rooms;
}

character_t** setUpcharacters(int numMonsters, player_t* player, int numRooms, room_t* rooms) {
    character_t** characters = setCharacters(numMonsters);
    characters[0] = (character_t*)player;
    for(int i = 1; i < numMonsters + 1; i++) {
        monster_t* monster = createMonster(rooms, numRooms);
        characters[i] = ((character_t*)monster);
    }

    return characters;
}

#pragma mark - Priority Queue

int32_t compareCharacters(const void *key, const void *with) {
    character_t** character1 = (character_t**) key;
    character_t** character2 = (character_t**) with;

    if(getTurn(*character1) - getTurn(*character2) != 0) {
        return getTurn(*character1) - getTurn(*character2);
    } else {
        return getPriority(*character1) - getPriority(*character2);
    }
}

void freeCharacter(void* key) {
    character_t** character = (character_t**) key;
    deleteCharacter(*character);
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

bool checkColor(string color) {
    if(color == "RED") {
        return true;
    } else if(color == "GREEN") {
        return true;
    } else if(color == "BLUE") {
        return true;
    } else if(color == "CYAN") {
        return true;
    } else if(color == "YELLOW") {
        return true;
    } else if(color == "MAGENTA") {
        return true;
    } else if(color == "WHITE") {
        return true;
    } else if(color == "BLACK") {
        return true;
    }

    return false;
}