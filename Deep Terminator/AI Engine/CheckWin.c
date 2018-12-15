//
//  CheckWin.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/15.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "board.h"
#include "ACautomaton.h"

extern struct node * rootBlackWin;
extern struct node * rootWhiteWin;

extern char *** getBoardStr(char * board);
extern int * recognize(char * pattern, struct node * root, int keycharCount);

int checkWhoWin(char * board) {
    int i, j;
    int * resultBlack = (int *)malloc(sizeof(int));
    int * resultWhite = (int *)malloc(sizeof(int));
    char *** tempBoardStr = getBoardStr(board);
    int * tempResultBlack;
    int * tempResultWhite;
    resultBlack[0] = 0;
    resultWhite[0] = 0;
    
    //横向
    for (i = 0; i < 15; i++) {
        tempResultBlack = recognize(tempBoardStr[0][i], rootBlackWin, 1);
        tempResultWhite = recognize(tempBoardStr[0][i], rootWhiteWin, 1);
        for (j = 0; j < 1; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
        free(tempBoardStr[0][i]);
    }
    free(tempBoardStr[0]);
    
    //纵向
    for (i = 0; i < 15; i++) {
        tempResultBlack = recognize(tempBoardStr[1][i], rootBlackWin, 1);
        tempResultWhite = recognize(tempBoardStr[1][i], rootWhiteWin, 1);
        for (j = 0; j < 1; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
        free(tempBoardStr[1][i]);
    }
    free(tempBoardStr[1]);
    
    //左上到右下
    for (i = 0; i < 29; i++) {
        tempResultBlack = recognize(tempBoardStr[2][i], rootBlackWin, 1);
        tempResultWhite = recognize(tempBoardStr[2][i], rootWhiteWin, 1);
        for (j = 0; j < 1; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
        free(tempBoardStr[2][i]);
    }
    free(tempBoardStr[2]);
    
    //右上到左下
    for (i = 0; i < 29; i++) {
        tempResultBlack = recognize(tempBoardStr[3][i], rootBlackWin, 1);
        tempResultWhite = recognize(tempBoardStr[3][i], rootWhiteWin, 1);
        for (j = 0; j < 1; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
        free(tempBoardStr[3][i]);
    }
    free(tempBoardStr[3]);
    free(tempBoardStr);
    
    if (resultBlack[0]) {
        free(resultBlack);
        free(resultWhite);
        return Black;
    }
    else if (resultWhite[0]) {
        free(resultBlack);
        free(resultWhite);
        return White;
    }
    else {
        free(resultBlack);
        free(resultWhite);
        return 0;
    }
}
