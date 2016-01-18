//
//  main.c
//  RogueLike
//
//  Created by Camden Voigt on 1/17/16.
//  Copyright © 2016 Camden Voigt. All rights reserved.
//

#include <stdio.h>
#include "Room.h"

int main(int argc, const char * argv[]) {
    
    Room room;
    
    room.height = 12;
    room.width = 4;
    
    drawRoom(room);
    printf("\n");
    return 0;
}
