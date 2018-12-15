//
//  main.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "board.h"
#include "settings.h"

int evaNodes = 0;

extern int cut;
extern char board[225];
extern int * pos;

extern void init(void);
extern void put(int index);
extern int intCount(int * array);
extern void removePiece(void);
extern void removeAllPieces(void);
extern int transCoordinateToIndex(char * position);
extern char * transIndexToCoordinate(int index);

extern int search(char * board, int color);
extern int checkWhoWin(char * board);
extern int checkForbidMove(char * board);
extern int checkRenjuWhoWin(char * board);

int main(void) {
    int whoWin = 0;
    int decidedIndex;
    char input[10];
    
    init();
    
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
                if (checkForbidMove(board))
                    printf("White wins! Black has made a forbidden move!\n");
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
