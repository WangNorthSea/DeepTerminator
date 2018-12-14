//
//  ChangeBoard.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "board.h"

extern char * append(char * array, int value);
extern int count(char * array);

void put(int index) {
    pos = append(pos, index);
    if (count(pos) % 2 != 0) {
        board[index] = Black;
    }
    else {
        board[index] = White;
    }
}

void removePiece(void) {
    int pieceCount = count(pos);
    board[pos[pieceCount - 1]] = Empty;
    pos[pieceCount - 1] = -1;
    pos = (char *)realloc(pos, pieceCount);
}
