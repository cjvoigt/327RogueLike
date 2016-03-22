//
//  Monster.h
//  RogueLike
//
//  Created by Camden Voigt on 2/16/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#ifndef monster_h
#define monster_h

#include "player.h"

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct monster {
} monster_t;

typedef struct position {
    int x;
    int y;
    int distance;
    binheap_node_t* heapNode;
} position_t;

void drawMonsterList(character_t* characters, int numMonsters); 

/* Monster Creation */
monster_t* createMonster(room_t* rooms, int numRooms);

/* Monster Movement */
void moveMonster(monster_t* monster, player_t* p);

/* Line of Sight */
void findLineOfSightMultiple(character_t* characters, int numChars, player_t* p, room_t* rooms, int numRooms);
void findLineOfSightSingle(character_t* character, int numChars, player_t* p, room_t* rooms, int numRooms);
    
/* Getters */
int getBehavior(const monster_t* monster);
int getLastX(const monster_t* monster);
int getLastY(const monster_t* monster);
int getVisible(const monster_t* monster);
    
/* Setters */
void setBehavior(monster_t* monster, int behavior);
void setLastX(monster_t* monster, int lastX);
void setLastY(monster_t* monster, int lastY);
void setVisible(monster_t* monster, int visible);

#ifdef __cplusplus
}

class Monster: public Character {
public:
    int behavior;
    int lastX;
    int lastY;
    int visible;
};
#endif

#endif /* Monster_h */
