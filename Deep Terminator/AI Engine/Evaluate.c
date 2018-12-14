//
//  Evaluate.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/14.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
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

int evaluate(char * board, int color) {
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
    
    scoreBlack = getScore(resultBlack);
    scoreWhite = getScore(resultWhite);
    
    free(resultBlack);
    free(resultWhite);
    free(tempBoardStr);
    return color == Black ? scoreBlack - scoreWhite : scoreWhite - scoreBlack;
}
