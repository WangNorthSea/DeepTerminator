//
//  board.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/20.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include <memory.h>

#define Empty 0
#define Black 1
#define White 2
#define Extra 3
#define Stop -1

char board[225];
int * pos;

int min(int a, int b) {
    return a - b < 0 ? a : b;
}

unsigned int * getPatternCodes(int index) {
    int i;
    int leftLimit = -(index % 15);
    int rightLimit = 14 + leftLimit;
    int upLimit = -(index / 15);
    int downLimit = 14 + upLimit;
    int sixteenUpLimit = -min(-leftLimit, -upLimit);
    int sixteenDownLimit = min(rightLimit, downLimit);
    int fourteenUpLimit = -min(rightLimit, -upLimit);
    int fourteenDownLimit = min(-leftLimit, downLimit);
    unsigned int * pcode = (unsigned int *)malloc(sizeof(unsigned int) * 4);
    memset(pcode, 0, sizeof(unsigned int) * 4);
    
    //横向
    for (i = 5; i >= -5; i--) {
        if (i < leftLimit || i > rightLimit)
            pcode[0] |= Extra << (-(i - 5) << 1);
        else
            pcode[0] |= board[index + i] << (-(i - 5) << 1);
    }
    
    //纵向
    for (i = 5; i >= -5; i--) {
        if (i < upLimit || i > downLimit)
            pcode[1] |= Extra << (-(i - 5) << 1);
        else
            pcode[1] |= board[index + 15 * i] << (-(i - 5) << 1);
    }
    
    //左上到右下
    for (i = 5; i >= -5; i--) {
        if (i < sixteenUpLimit || i > sixteenDownLimit)
            pcode[2] |= Extra << (-(i - 5) << 1);
        else
            pcode[2] |= board[index + 16 * i] << (-(i - 5) << 1);
    }
    
    //右上到左下
    for (i = 5; i >= -5; i--) {
        if (i < fourteenUpLimit || i > fourteenDownLimit)
            pcode[3] |= Extra << (-(i - 5) << 1);
        else
            pcode[3] |= board[index + 14 * i] << (-(i - 5) << 1);
    }
    
    return pcode;
}
