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

int patternScore[19] = {
    10000,                       //Consecutive Five
    5000,                         //Live Four
    120, 120, 120, 120, 120,      //Rush Four
    95, 95, 95,                  //Live Three
    35, 35, 35, 35, 35, 35,      //Sleep Three
    5, 5, 5                   //Live Two
};

int getScore(int * result) {
    int i;
    int score = 0;
    
    for (i = 0; i < 19; i++)
        score += result[i] * patternScore[i];
    
    return score;
}

char *** getBoardStr(char * board) {
    int i, j;
    char *** boardStr = (char ***)malloc(sizeof(char **) * 4);
    
    //boardStr[i][j][k]  i代表方向，j代表该方向第j + 1个字符串，k代表该字符串中某个字符
    
    for (i = 0; i < 4; i++) {
        boardStr[i] = (char **)malloc(sizeof(char *) * 29);
        for (j = 0; j < 29; j++) {
            boardStr[i][j] = (char *)malloc(16);
            boardStr[i][j][15] = -1;
        }
    }
    
    //横向
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 15; j++)
            boardStr[0][i][j] = board[i * 15 + j];
    }
    
    //纵向
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 15; j++)
            boardStr[1][i][j] = board[i + j * 15];
    }
    
    //左上到右下
    for (i = 0; i < 15; i++) {
        for (j = 0; j <= i; j++)
            boardStr[2][i][j] = board[14 - i + j * 16];
        if (i < 14)
            boardStr[2][i][j] = -1;
    }//右半部分
    for (i = 1; i < 15; i++) {
        for (j = 0; j <= 14 - i; j++)
            boardStr[2][i + 14][j] = board[i * 15 + j * 16];
        boardStr[2][i + 14][j] = -1;
    }//左半部分
    
    //右上到左下
    for (i = 0; i < 15; i++) {
        for (j = 0; j <= i; j++)
            boardStr[3][i][j] = board[i + j * 14];
        if (i < 14)
            boardStr[3][i][j] = -1;
    }//左半部分
    for (i = 1; i < 15; i++) {
        for (j = 0; j <= 14 - i; j++)
            boardStr[3][i + 14][j] = board[14 + i * 15 + j * 14];
        boardStr[3][i + 14][j] = -1;
    }//右半部分
    
    return boardStr;
}

int evaluate(char * board, int color) {
    int i, j;
    int scoreBlack = 0, scoreWhite = 0;
    int resultBlack[19] = {0};
    int resultWhite[19] = {0};
    char *** tempBoardStr = getBoardStr(board);
    int * tempResultBlack;
    int * tempResultWhite;
    
    //横向
    for (i = 0; i < 15; i++) {
        tempResultBlack = recognize(tempBoardStr[0][i], rootBlack, 19);
        tempResultWhite = recognize(tempBoardStr[0][i], rootWhite, 19);
        for (j = 0; j < 19; j++) {
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
        tempResultBlack = recognize(tempBoardStr[1][i], rootBlack, 19);
        tempResultWhite = recognize(tempBoardStr[1][i], rootWhite, 19);
        for (j = 0; j < 19; j++) {
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
        tempResultBlack = recognize(tempBoardStr[2][i], rootBlack, 19);
        tempResultWhite = recognize(tempBoardStr[2][i], rootWhite, 19);
        for (j = 0; j < 19; j++) {
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
        tempResultBlack = recognize(tempBoardStr[3][i], rootBlack, 19);
        tempResultWhite = recognize(tempBoardStr[3][i], rootWhite, 19);
        for (j = 0; j < 19; j++) {
            resultBlack[j] += tempResultBlack[j];
            resultWhite[j] += tempResultWhite[j];
        }
        free(tempResultBlack);
        free(tempResultWhite);
        free(tempBoardStr[3][i]);
    }
    free(tempBoardStr[3]);
    
    free(tempBoardStr);
    
    //下一个落子方是黑棋，说明最后落子方是白棋，所以当前针对白棋进行评价
    if (color == White) {
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
    else {  //下一个落子方是白棋，说明最后落子方是黑棋，所以当前针对黑棋进行评价
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
    
    return color == Black ? scoreBlack - scoreWhite : scoreWhite - scoreBlack;
}
