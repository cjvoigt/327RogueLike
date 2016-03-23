//
//  Character.hpp
//  327RoguelikeC++
//
//  Created by Camden Voigt on 3/21/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#ifndef character_h
#define character_h

#include <stdio.h>
#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct character {
} character_t;

//Getters
int getX(const character_t* c);
int getY(const character_t* c);
int getTurn(const character_t* c);
int getPriority(const character_t* c);
int getSpeed(const character_t* c);
int getDead(const character_t* c);
characterTag_t getType(const character_t* c);

character_t** setCharacters(int numMonsters);
void deleteCharacter(character_t* c);
void deleteCharacterArray(character_t** c);
int getBehavior(const monster_t* monster);

//Setters
void setX(character_t* c, int x);
void setY(character_t* c, int y);
void setTurn(character_t* c, int turn);
void setPriority(character_t* c, int priority);
void setSpeed(character_t* c, int speed);
void setDead(character_t* c, int dead);
void setType(character_t* c, characterTag_t type);

#ifdef __cplusplus
}

class Character {
public:
    int x;
    int y;
    int turn;
    int priority;
    int speed;
    int dead;
    characterTag_t type;
};
#endif

#endif /* Character_hpp */