//
//  ChangeBoard.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/AI Engine/board.h"
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/AI Engine/array.h"

void put(int index) {
    pos = append(pos, index);
    if (intCount(pos) % 2 != 0)
        putPiece(board, index, Black);
    else
        putPiece(board, index, White);
}

void removePiece(void) {
    int piececharCount = intCount(pos);
    takePiece(board, pos[piececharCount - 1]);
    pos[piececharCount - 1] = -1;
    pos = (int *)realloc(pos, sizeof(int) * piececharCount);
}

void removeAllPieces(void) {
    int i, j;
    pos = (int *)realloc(pos, sizeof(int));
    pos[0] = -1;
    for (i = 0; i < 225; i++)
        board[i] = Empty;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 10; j++)
            patCurrent.pat[i][j] = 0;
    }
    moveCount = 0;
}
