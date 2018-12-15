//
//  ChangeBoard.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "board.h"

extern char board[225];
extern int * pos;

extern int * append(int * array, int value);
extern int intCount(int * array);

void put(int index) {
    pos = append(pos, index);
    if (intCount(pos) % 2 != 0)
        board[index] = Black;
    else
        board[index] = White;
}

void removePiece(void) {
    int piececharCount = intCount(pos);
    board[pos[piececharCount - 1]] = Empty;
    pos[piececharCount - 1] = -1;
    pos = (int *)realloc(pos, sizeof(int) * piececharCount);
}

void removeAllPieces(void) {
    int i;
    pos = (int *)realloc(pos, sizeof(int));
    pos[0] = -1;
    for (i = 0; i < 225; i++)
        board[i] = Empty;
}
