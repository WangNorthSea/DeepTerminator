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
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/AI Engine/init.h"

void put(int index) {
    pos = append(pos, index);
    if (intCount(pos) % 2 != 0)
        putPiece(board, index, Black);
    else
        putPiece(board, index, White);
}

void removePiece(void) {
    int piececharCount = intCount(pos);
    if (piececharCount % 2 != 0)
        takePiece(board, pos[piececharCount - 1], Black);
    else
        takePiece(board, pos[piececharCount - 1], White);
    pos[piececharCount - 1] = -1;
    pos = (int *)realloc(pos, sizeof(int) * piececharCount);
}
