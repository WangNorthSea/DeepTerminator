//
//  CheckWin.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/15.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "board.h"
#include "AC.h"
#include "init.h"
#include "evaluate.h"

int checkWhoWin(char * board) {
    int i, j;
    int resultBlack[1];
    int resultWhite[1];
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
    for (i = 15; i < 29; i++)
        free(tempBoardStr[0][i]);
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
    for (i = 15; i < 29; i++)
        free(tempBoardStr[1][i]);
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
    
    if (resultBlack[0])
        return Black;
    else if (resultWhite[0])
        return White;
    else
        return 0;
}

int checkRenjuWhoWin(char * board) {
    int i, j;
    int resultBlack[4] = {0};
    int resultWhite[1];
    char *** tempBoardStr = getBoardStr(board);
    int * tempResultBlack;
    int * tempResultWhite;
    resultWhite[0] = 0;
    
    //横向
    for (i = 0; i < 15; i++) {
        tempResultBlack = recognize(tempBoardStr[0][i], rootBlackRenjuWin, 4);
        tempResultWhite = recognize(tempBoardStr[0][i], rootWhiteWin, 1);
        for (j = 0; j < 4; j++)
            resultBlack[j] += tempResultBlack[j];
        resultWhite[0] += tempResultWhite[0];
        free(tempResultBlack);
        free(tempResultWhite);
        free(tempBoardStr[0][i]);
    }
    for (i = 15; i < 29; i++)
        free(tempBoardStr[0][i]);
    free(tempBoardStr[0]);
    
    //纵向
    for (i = 0; i < 15; i++) {
        tempResultBlack = recognize(tempBoardStr[1][i], rootBlackRenjuWin, 4);
        tempResultWhite = recognize(tempBoardStr[1][i], rootWhiteWin, 1);
        for (j = 0; j < 4; j++)
            resultBlack[j] += tempResultBlack[j];
        resultWhite[0] += tempResultWhite[0];
        free(tempResultBlack);
        free(tempResultWhite);
        free(tempBoardStr[1][i]);
    }
    for (i = 15; i < 29; i++)
        free(tempBoardStr[1][i]);
    free(tempBoardStr[1]);
    
    //左上到右下
    for (i = 0; i < 29; i++) {
        tempResultBlack = recognize(tempBoardStr[2][i], rootBlackRenjuWin, 4);
        tempResultWhite = recognize(tempBoardStr[2][i], rootWhiteWin, 1);
        for (j = 0; j < 4; j++)
            resultBlack[j] += tempResultBlack[j];
        resultWhite[0] += tempResultWhite[0];
        free(tempResultBlack);
        free(tempResultWhite);
        free(tempBoardStr[2][i]);
    }
    free(tempBoardStr[2]);
    
    //右上到左下
    for (i = 0; i < 29; i++) {
        tempResultBlack = recognize(tempBoardStr[3][i], rootBlackRenjuWin, 4);
        tempResultWhite = recognize(tempBoardStr[3][i], rootWhiteWin, 1);
        for (j = 0; j < 4; j++)
            resultBlack[j] += tempResultBlack[j];
        resultWhite[0] += tempResultWhite[0];
        free(tempResultBlack);
        free(tempResultWhite);
        free(tempBoardStr[3][i]);
    }
    free(tempBoardStr[3]);
    free(tempBoardStr);
    
    if (resultBlack[0] + resultBlack[1] + resultBlack[2] + resultBlack[3])
        return Black;
    else if (resultWhite[0])
        return White;
    else
        return 0;
}
