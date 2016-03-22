//
//  player.h
//  RogueLike
//
//  Created by Camden Voigt on 2/3/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#ifndef player_h
#define player_h

#include <stdio.h>
#include <stdint.h>

#include "binheap.h"
#include "character.h"
#include "Dungeon.h"

# ifdef __cplusplus
extern "C" {
# endif

typedef struct player {
} player_t;

/* Player Creation */
player_t* createPlayer(room_t* rooms);
void changePos(player_t* player, room_t* rooms);

/* Player Movement */
void movePlayer(player_t* player, int direction);
void swapPlayer(player_t* player, int newX, int newY);

# ifdef __cplusplus
}

class Player: public Character {
public:
        
};
# endif

#endif /* player_h */
