//
//  board.h
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#ifndef board_h
#define board_h

#define Empty 0
#define Black 1
#define White 2
#define Extra 3
#define Stop -1

struct pattern {
    int pat[2][10];   //第一个下标0代表黑子棋型，1代表白子棋型
};

struct candPoint {
    int pat[2][10];
};

extern char board[225];
extern int * pos;
extern unsigned short int moveCount;
extern struct pattern patCurrent;
extern struct pattern patHistory[256];
extern unsigned char refreshed[225];
extern struct candPoint candidates[225];

extern unsigned int getPatternCode(char * board, int index, int direction);
extern void putPiece(char * board, int index, int color);
extern void takePiece(char * board, int index, int color);

#endif /* board_h */
