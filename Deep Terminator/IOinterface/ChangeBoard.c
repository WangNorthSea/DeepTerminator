//
//  ChangeBoard.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "../AIEngine/board.h"
#include "../AIEngine/array.h"
#include "../AIEngine/init.h"

/*
 本函数用于在棋盘上落子以及将落子坐标按落子顺序添加到pos数组中
 */
void put(int index) {
    pos = append(pos, index);
    if (intCount(pos) % 2 != 0)
        putPiece(board, index, Black);
    else
        putPiece(board, index, White);
}

/*
 本函数用于从棋盘上移除最后落的一个棋子并且将其从pos数组中删除
 */
void removePiece(void) {
    int piececharCount = intCount(pos);
    if (piececharCount % 2 != 0)
        takePiece(board, pos[piececharCount - 1], Black);
    else
        takePiece(board, pos[piececharCount - 1], White);
    pos[piececharCount - 1] = -1;
    pos = (int *)realloc(pos, sizeof(int) * piececharCount);
}
