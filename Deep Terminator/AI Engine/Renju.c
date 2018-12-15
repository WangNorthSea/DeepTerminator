//
//  Renju.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/15.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "ACautomaton.h"

extern struct node * rootForbidMove;

extern char *** getBoardStr(char * board);
extern int * recognize(char * pattern, struct node * root, int keycharCount);

int checkForbidMove(char * board) {
    int i, j;
    int checkResult[9] = {0};
    char *** tempBoardStr = getBoardStr(board);
    int * tempResult;
    
    //横向
    for (i = 0; i < 15; i++) {
        tempResult = recognize(tempBoardStr[0][i], rootForbidMove, 9);
        for (j = 0; j < 9; j++)
            checkResult[j] += tempResult[j];
        free(tempResult);
        free(tempBoardStr[0][i]);
    }
    for (i = 15; i < 29; i++)
        free(tempBoardStr[0][i]);
    free(tempBoardStr[0]);
    
    //纵向
    for (i = 0; i < 15; i++) {
        tempResult = recognize(tempBoardStr[1][i], rootForbidMove, 9);
        for (j = 0; j < 9; j++)
            checkResult[j] += tempResult[j];
        free(tempResult);
        free(tempBoardStr[1][i]);
    }
    for (i = 15; i < 29; i++)
        free(tempBoardStr[1][i]);
    free(tempBoardStr[1]);
    
    //左上到右下
    for (i = 0; i < 29; i++) {
        tempResult = recognize(tempBoardStr[2][i], rootForbidMove, 9);
        for (j = 0; j < 9; j++)
            checkResult[j] += tempResult[j];
        free(tempResult);
        free(tempBoardStr[2][i]);
    }
    free(tempBoardStr[2]);
    
    //右上到左下
    for (i = 0; i < 29; i++) {
        tempResult = recognize(tempBoardStr[3][i], rootForbidMove, 9);
        for (j = 0; j < 9; j++)
            checkResult[j] += tempResult[j];
        free(tempResult);
        free(tempBoardStr[3][i]);
    }
    free(tempBoardStr[3]);
    free(tempBoardStr);
    
    if (checkResult[0])
        return 1;
    else if (checkResult[1] + checkResult[2] + checkResult[3] + checkResult[4] + checkResult[5] >= 2)
        return 1;
    else if (checkResult[6] + checkResult[7] + checkResult[8] >= 2)
        return 1;
    else
        return 0;
}
