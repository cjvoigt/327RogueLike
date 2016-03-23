//
//  Character.cpp
//  327RoguelikeC++
//
//  Created by Camden Voigt on 3/21/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include "character.h"

//Getters
int getX(const character_t *c) {
    return ((Character *)c)->x;
}

int getY(const character_t *c) {
    return ((Character *)c)->y;
}

int getTurn(const character_t *c) {
    return ((Character *)c)->turn;
}

int getPriority(const character_t *c) {
    return ((Character *)c)->priority;
}

int getSpeed(const character_t *c) {
    return ((Character *)c)->speed;
}

int getDead(const character_t *c) {
    return ((Character *)c)->dead;
}

characterTag_t getType(const character_t* c) {
    return ((Character *) c)->type;
}

//Setters
void setX(character_t *c, int x) {
    ((Character*) c)->x = x;
}

void setY(character_t *c, int y) {
    ((Character*) c)->y = y;
}

void setTurn(character_t *c, int turn) {
    ((Character*) c)->turn = turn;
}

void setPriority(character_t *c, int priority) {
    ((Character*) c)->priority = priority;
}

void setSpeed(character_t *c, int speed) {
    ((Character*) c)->speed = speed;
}

void setDead(character_t *c, int dead) {
    ((Character*) c)->dead = dead;
}

void setType(character_t* c, characterTag_t type) {
    ((Character *) c)->type = type;
}

character_t** setCharacters(int numMonsters) {
    Character** characters = new Character*[numMonsters + 1];
    return (character_t**) characters;
}