//
//  utils.h
//  RogueLike
//
//  Created by Camden Voigt on 2/16/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#ifndef utils_h
#define utils_h

#include <ncurses.h>
#include <curses.h>

#include "binheap.h"

#define TRUE 1
#define FALSE 0
#define INFINITY 255

#define MONSTER0 0x0
#define MONSTER1 0x1
#define MONSTER2 0x2
#define MONSTER3 0x3
#define MONSTER4 0x4
#define MONSTER5 0x5
#define MONSTER6 0x6
#define MONSTER7 0x7
#define MONSTER8 0x8
#define MONSTER9 0x9
#define MONSTERA 0xa
#define MONSTERB 0xb
#define MONSTERC 0xc
#define MONSTERD 0xd
#define MONSTERE 0xe
#define MONSTERF 0xf

typedef struct room {
    int x;
    int y;
    int width;
    int height;
    int centerX;
    int centerY;
} room_t;

typedef enum characterTag {
    none,
    pc,
    mon
} characterTag_t;

#endif /* utils_h */
