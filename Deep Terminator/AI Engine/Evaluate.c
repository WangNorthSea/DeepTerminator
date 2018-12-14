//
//  Evaluate.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/14.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include <memory.h>
#include "board.h"
#include "ACautomaton.h"

extern struct node * rootBlack;
extern struct node * rootWhite;
extern int * recognize(char * pattern, struct node * root, int keyCount);

int patternScore[18] = {
    10000,                       //Consecutive Five
    500,                         //Live Four
    100, 100, 100, 100,          //Rush Four
    80, 80, 80,                  //Live Three
    40, 40, 40, 40, 40, 40,      //Sleep Three
    10, 10, 10                   //Live Two
};

int getScore(int * result) {
    int i;
    int score = 0;
    
    for (i = 0; i < 18; i++)
        score += result[i] * patternScore[i];
    
    return score;
}

int evaluate(char * board, int color, int nextColor) {
    int i, j;
    int scoreBlack = 0, scoreWhite = 0;
    int * resultBlack = (int *)malloc(sizeof(int) * 18);
    int * resultWhite = (int *)malloc(sizeof(int) * 18);
    char * tempBoardStr = (char *)malloc(16);
    int * tempResultBlack;
    int * tempResultWhite;
    resultBlack = memset(resultBlack, 0, sizeof(int) * 18);
    resultWhite = memset(resultWhite, 0, sizeof(int) * 18);
    tempBoardStr[15] = -1;
    
    //横向
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 15; j++)
            tempBoardStr[j] = board[i * 15 + j];
        tempResultBlack = recognize(tempBoardStr, rootBlack, 18);
        tempResultWhite = recognize(tempBoardStr, rootWhite, 18);
        for (j = 0; j < 18; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
    }
    
    //纵向
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 15; j++)
            tempBoardStr[j] = board[i + j * 15];
        tempResultBlack = recognize(tempBoardStr, rootBlack, 18);
        tempResultWhite = recognize(tempBoardStr, rootWhite, 18);
        for (j = 0; j < 18; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
    }
    
    //左上到右下
    for (i = 0; i < 15; i++) {
        for (j = 0; j <= i; j++)
            tempBoardStr[j] = board[14 - i + j * 16];
        if (i < 14)
            tempBoardStr[j] = -1;
        tempResultBlack = recognize(tempBoardStr, rootBlack, 18);
        tempResultWhite = recognize(tempBoardStr, rootWhite, 18);
        for (j = 0; j < 18; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
    }//右半部分
    for (i = 1; i < 15; i++) {
        for (j = 0; j <= 14 - i; j++)
            tempBoardStr[j] = board[i * 15 + j * 16];
        tempBoardStr[j] = -1;
        tempResultBlack = recognize(tempBoardStr, rootBlack, 18);
        tempResultWhite = recognize(tempBoardStr, rootWhite, 18);
        for (j = 0; j < 18; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
    }//左半部分
    
    //右上到左下
    for (i = 0; i < 15; i++) {
        for (j = 0; j <= i; j++)
            tempBoardStr[j] = board[i + j * 14];
        if (i < 14)
            tempBoardStr[j] = -1;
        tempResultBlack = recognize(tempBoardStr, rootBlack, 18);
        tempResultWhite = recognize(tempBoardStr, rootWhite, 18);
        for (j = 0; j < 18; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
    }//左半部分
    for (i = 1; i < 15; i++) {
        for (j = 0; j <= 14 - i; j++)
            tempBoardStr[j] = board[14 + i * 15 + j * 14];
        tempBoardStr[j] = -1;
        tempResultBlack = recognize(tempBoardStr, rootBlack, 18);
        tempResultWhite = recognize(tempBoardStr, rootWhite, 18);
        for (j = 0; j < 18; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
    }//右半部分
    
    //下一个落子方是黑棋，说明最后落子方是白棋，且由于本函数只在最深层被调用，所以当前针对白棋进行评价
    if (nextColor == Black) {
        if (resultBlack[0])       //黑棋在白棋落最后一子前已经五连，说明黑棋已胜利
            return -10000;
        else if (resultWhite[0])  //黑棋没有五连，白棋五连，说明白棋已胜利。
            return 10000;
        else if (resultBlack[1] || resultBlack[2] || resultBlack[3] || resultBlack[4] || resultBlack[5])
            return -10000;    //黑白棋均没有五连，但黑棋有活四或者冲四，下一个落子方是黑棋，说明黑棋已胜利
        else if (resultWhite[1])
            return 10000;     //黑棋没有四，白棋有活四，说明白棋已胜利
        else if (!resultWhite[2] && !resultWhite[3] && !resultWhite[4] && !resultWhite[5] && (resultBlack[6] || resultBlack[7] || resultBlack[8]))
            return -10000;    //白棋没有四，黑棋有活三，说明黑棋已胜利
    }
    else {
        if (resultWhite[0])
            return -10000;    //白棋在黑棋落最后一子前就已经五连，说明白棋已胜利
        else if (resultBlack[0])
            return 10000;     //白棋没有五连，黑棋五连，说明黑棋已胜利
        else if (resultWhite[1] || resultWhite[2] || resultWhite[3] || resultWhite[4] || resultWhite[5])
            return -10000;    //黑白棋均没有五连，但白棋有活四或冲四，下一个落子方是白棋，说明白棋已胜利
        else if (resultBlack[1])
            return 10000;     //白棋没有四，黑棋有活四，说明黑棋已胜利
        else if (!resultBlack[2] && !resultBlack[3] && !resultBlack[4] && !resultBlack[5] && (resultWhite[6] || resultWhite[7] || resultWhite[8]))
            return -10000;    //黑棋没有四，白棋有活三，说明白棋已胜利
    }
    
    scoreBlack = getScore(resultBlack);
    scoreWhite = getScore(resultWhite);
    
    free(resultBlack);
    free(resultWhite);
    free(tempBoardStr);
    return color == Black ? scoreBlack - scoreWhite : scoreWhite - scoreBlack;
}
