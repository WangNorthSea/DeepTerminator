//
//  Initialize.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "board.h"

extern char board[225];
extern char * pos;

void init(void) {
    int i;
    for (i = 0; i < 225; i++)
        board[i] = Empty;
    pos = (char *)malloc(1);
    pos[0] = -1;
}
