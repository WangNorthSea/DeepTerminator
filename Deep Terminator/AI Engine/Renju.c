//
//  Renju.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/15.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "init.h"
#include "board.h"

int checkForbidMove(char * board, int index) {
    int i, count = 0;
    unsigned char pats[4];
    pats[0] = patMap[getPatternCode(board, index, 0)];
    pats[1] = patMap[getPatternCode(board, index, 1)];
    pats[2] = patMap[getPatternCode(board, index, 2)];
    pats[3] = patMap[getPatternCode(board, index, 3)];
    
    for (i = 0; i < 4; i++) {
        if ((pats[i] & 15) == 5)
            count++;
    }
    if (count >= 2)
        return 1;
    
    count = 0;
    for (i = 0; i < 4; i++) {
        if ((pats[i] & 15) == 6 || (pats[i] & 15) == 7)
            count++;
    }
    if (count >= 2)
        return 1;
    
    return 0;
}
