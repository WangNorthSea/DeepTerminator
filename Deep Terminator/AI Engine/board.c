//
//  board.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/20.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include <memory.h>
#include "init.h"
#include "settings.h"
#include "zobrist.h"

#define Empty 0
#define Black 1
#define White 2
#define Extra 3

char board[225];
int * pos;

struct pattern {
    int pat[2][10];   //第一个下标0代表黑子棋型，1代表白子棋型
};

unsigned short int moveCount = 0;
struct pattern patCurrent;
struct pattern patHistory[256];

int minimum(int a, int b) {
    return a - b < 0 ? a : b;
}

unsigned int getPatternCode(char * board, int index, int direction) {
    int i;
    unsigned int pcode = 0;
    
    if (direction == 0) {
        //横向
        int leftLimit = -(index % 15);
        int rightLimit = 14 + leftLimit;
        for (i = 5; i >= -5; i--) {
            if (i < leftLimit || i > rightLimit)
                pcode |= Extra << (-(i - 5) << 1);
            else
                pcode |= board[index + i] << (-(i - 5) << 1);
        }
    }
    
    if (direction == 1) {
        //纵向
        int upLimit = -(index / 15);
        int downLimit = 14 + upLimit;
        for (i = 5; i >= -5; i--) {
            if (i < upLimit || i > downLimit)
                pcode |= Extra << (-(i - 5) << 1);
            else
                pcode |= board[index + 15 * i] << (-(i - 5) << 1);
        }
    }
    
    if (direction == 2) {
        //左上到右下
        int sixteenUpLimit = -minimum(index % 15, index / 15);
        int sixteenDownLimit = minimum(14 - index % 15, 14 - index / 15);
        for (i = 5; i >= -5; i--) {
            if (i < sixteenUpLimit || i > sixteenDownLimit)
                pcode |= Extra << (-(i - 5) << 1);
            else
                pcode |= board[index + 16 * i] << (-(i - 5) << 1);
        }
    }
    
    if (direction == 3) {
        //右上到左下
        int fourteenUpLimit = -minimum(14 - index % 15, index / 15);
        int fourteenDownLimit = minimum(index % 15, 14 - index / 15);
        for (i = 5; i >= -5; i--) {
            if (i < fourteenUpLimit || i > fourteenDownLimit)
                pcode |= Extra << (-(i - 5) << 1);
            else
                pcode |= board[index + 14 * i] << (-(i - 5) << 1);
        }
    }
    
    return pcode;
}

//落子时正常调用即可，即时改变局面双方棋型
void putPiece(char * board, int index, int color) {
#ifdef HASH
    hashKey ^= zobristMap[index][color - 1];
#endif
    int i;
    unsigned char pats[4];
    unsigned char pats2[12] = {9};
    int leftLimit = index % 15;
    int upLimit = index / 15;
    pats[0] = patMap[getPatternCode(board, index, 0)];
    pats[1] = patMap[getPatternCode(board, index, 1)];
    pats[2] = patMap[getPatternCode(board, index, 2)];
    pats[3] = patMap[getPatternCode(board, index, 3)];  //patMap中高四位表示白子棋型，低四位表示黑子棋型
    
    //落子之前在index处形成的棋型被破坏
    for (i = 0; i < 4; i++) {
        patCurrent.pat[0][pats[i] & 15]--;
        patCurrent.pat[1][pats[i] >> 4]--;
    }
    
    board[index] = color;
    pats2[0] = patMap[getPatternCode(board, index, 0)];
    pats2[1] = patMap[getPatternCode(board, index, 1)];
    pats2[2] = patMap[getPatternCode(board, index, 2)];
    pats2[3] = patMap[getPatternCode(board, index, 3)];
    
    if (leftLimit)
        pats2[4] = patMap[getPatternCode(board, index - 1, 0)];
    if (14 - leftLimit)
        pats2[5] = patMap[getPatternCode(board, index + 1, 0)];
    if (upLimit)
        pats2[6] = patMap[getPatternCode(board, index - 15, 1)];
    if (14 - upLimit)
        pats2[7] = patMap[getPatternCode(board, index + 15, 1)];
    if (leftLimit && upLimit)
        pats2[8] = patMap[getPatternCode(board, index - 16, 2)];
    if (14 - leftLimit && 14 - upLimit)
        pats2[9] = patMap[getPatternCode(board, index + 16, 2)];
    if (14 - leftLimit && upLimit)
        pats2[10] = patMap[getPatternCode(board, index - 14, 3)];
    if (leftLimit && 14 - upLimit)
        pats2[11] = patMap[getPatternCode(board, index + 14, 3)];
    
    //落子之后在index处形成新棋型
    if (color == Black) {
        for (i = 0; i < 4; i++)
            patCurrent.pat[0][pats2[i] & 15]++;
        for (i = 4; i < 12; i++) {
            //对手落子前是眠三，落子后是活三，则为误判
            if ((pats2[i] >> 4) == 5 && ((pats[(i - 4) / 2] >> 4)) == 4) {
                patCurrent.pat[1][4]++;
                continue;
            }
            patCurrent.pat[1][pats2[i] >> 4]++;
        }
    }
    else {
        for (i = 0; i < 4; i++)
            patCurrent.pat[1][pats2[i] >> 4]++;
        for (i = 4; i < 12; i++) {
            //对手落子前是眠三，落子后是活三，则为误判
            if ((pats2[i] & 15) == 5 && (pats[(i - 4) / 2] & 15) == 4) {
                patCurrent.pat[0][4]++;
                continue;
            }
            patCurrent.pat[0][pats2[i] & 15]++;
        }
    }
    
    moveCount++;
    patHistory[moveCount] = patCurrent;
}

void takePiece(char * board, int index, int color) {
#ifdef HASH
    hashKey ^= zobristMap[index][color - 1];
#endif
    moveCount--;
    board[index] = Empty;
    patCurrent = patHistory[moveCount];
}
