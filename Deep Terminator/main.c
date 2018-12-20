//
//  main.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//


//TODO: 1.寻找更科学的评分方式 2.把禁手识别函数改成只识别新落子的四个方向

#include <stdio.h>
#include <string.h>
#include "AI Engine/board.h"
#include "AI Engine/array.h"
#include "AI Engine/settings.h"
#include "AI Engine/init.h"
#include "AI Engine/search.h"
#include "AI Engine/win.h"
#include "AI Engine/renju.h"
#include "IO interface/changeBoard.h"
#include "IO interface/trans.h"

int evaNodes = 0;

int main(void) {
    int whoWin = 0;
    int decidedIndex;
    char input[10];
    
    printf("Initializing...Please wait...\n");
    init();
    printf("Initialization finished!\n");
    
    while (1) {
        scanf("%s", input);
        if (!strcmp(input, "calc")) {
            if (intCount(pos) % 2 != 0)
                decidedIndex = search(board, White);
            else
                decidedIndex = search(board, Black);
            put(decidedIndex);
            
            printf("evaluated nodes = %d\n", evaNodes);
            printf("alphaBeta cut = %d\n", cut);
            evaNodes = 0;
            cut = 0;
            
            if (Renju) {
                whoWin = checkRenjuWhoWin(board);
                if (whoWin == Black) {
                    printf("Black wins!\n");
                    removeAllPieces();
                }
                else if (whoWin == White) {
                    printf("White wins!\n");
                    removeAllPieces();
                }
            }
            else {
                whoWin = checkWhoWin(board);
                if (whoWin == Black) {
                    printf("Black wins!\n");
                    removeAllPieces();
                }
                else if (whoWin == White) {
                    printf("White wins!\n");
                    removeAllPieces();
                }
            }
        }
        else if (!strcmp(input, "exit"))
            break;
        else if (!strcmp(input, "remove"))
            removePiece();
        else {
            put(transCoordinateToIndex(input));
            
            if (Renju) {
                if (checkForbidMove(board)) {
                    printf("White wins! Black has made a forbidden move!\n");
                    removeAllPieces();
                }
                else {
                    whoWin = checkRenjuWhoWin(board);
                    if (whoWin == Black) {
                        printf("Black wins!\n");
                        removeAllPieces();
                    }
                    else if (whoWin == White) {
                        printf("White wins!\n");
                        removeAllPieces();
                    }
                }
            }
            else {
                whoWin = checkWhoWin(board);
                if (whoWin == Black) {
                    printf("Black wins!\n");
                    removeAllPieces();
                }
                else if (whoWin == White) {
                    printf("White wins!\n");
                    removeAllPieces();
                }
            }
        }
    }
    
    return 0;
}
