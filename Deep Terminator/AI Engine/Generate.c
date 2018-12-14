//
//  Generate.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/14.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include <memory.h>
#include "board.h"
#include "settings.h"
#include "ACautomaton.h"

extern int intCount(int * array);
extern int getScore(int * result);
extern int * append(int * array, int value);
extern int * recognize(char * pattern, struct node * root, int keycharCount);

extern int patternScore[18];
extern struct node * rootBlack;
extern struct node * rootWhite;

int min(int a, int b) { return (a - b <= 0) ? a : b; }

int * findSpace(char * board) {
    int i, j, k;
    int * indexArray = (int *)malloc(sizeof(int));
    indexArray[0] = -1;
    
    for (i = 0; i < 225; i++) {
        if ((board[i] == White) || (board[i] == Black)) {
            
            if (i % 15 < radius) {
                for (j = 1; j <= radius; j++) {
                    if (j <= (i % 15)) {
                        if ((board[i - j] == White) || (board[i - j] == Black)) { continue; }
                        for (k = 0; k < intCount(indexArray); k++) {
                            if (indexArray[k] == (i - j)) { break; }
                        }
                        if (k == intCount(indexArray)) {
                            indexArray = append(indexArray, i - j);
                        }
                    }
                }
            }
            else {
                for (j = 1; j <= radius; j++) {
                    if ((board[i - j] == White) || (board[i - j] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i - j)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i - j);
                    }
                }
            }
            
            if (14 - (i % 15) < radius) {
                for (j = 1; j <= radius; j++) {
                    if (j <= (14 - (i % 15))) {
                        if ((board[i + j] == White) || (board[i + j] == Black)) { continue; }
                        for (k = 0; k < intCount(indexArray); k++) {
                            if (indexArray[k] == (i + j)) { break; }
                        }
                        if (k == intCount(indexArray)) {
                            indexArray = append(indexArray, i + j);
                        }
                    }
                }
            }
            else {
                for (j = 1; j <= radius; j++) {
                    if ((board[i + j] == White) || (board[i + j] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i + j)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i + j);
                    }
                }
            }
            
            if (i / 15 < radius) {
                for (j = 1; j <= radius; j++) {
                    if (j <= (i / 15)) {
                        if ((board[i - j * 15] == White) || (board[i - j * 15] == Black)) { continue; }
                        for (k = 0; k < intCount(indexArray); k++) {
                            if (indexArray[k] == (i - j * 15)) { break; }
                        }
                        if (k == intCount(indexArray)) {
                            indexArray = append(indexArray, i - j * 15);
                        }
                    }
                }
            }
            else {
                for (j = 1; j <= radius; j++) {
                    if ((board[i - j * 15] == White) || (board[i - j * 15] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i - j * 15)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i - j * 15);
                    }
                }
            }
            
            if (14 - i / 15 < radius) {
                for (j = 1; j <= radius; j++) {
                    if (j <= (14 - i / 15)) {
                        if ((board[i + j * 15] == White) || (board[i + j * 15] == Black)) { continue; }
                        for (k = 0; k < intCount(indexArray); k++) {
                            if (indexArray[k] == (i + j * 15)) { break; }
                        }
                        if (k == intCount(indexArray)) {
                            indexArray = append(indexArray, i + j * 15);
                        }
                    }
                }
            }
            else {
                for (j = 1; j <= radius; j++) {
                    if ((board[i + j * 15] == White) || (board[i + j * 15] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i + j * 15)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i + j * 15);
                    }
                }
            }
            
            if (min(i % 15, i / 15) < radius) {
                for (j = 1; j <= radius; j++) {
                    if (j <= min(i % 15, i / 15)) {
                        if ((board[i - j * 16] == White) || (board[i - j * 16] == Black)) { continue; }
                        for (k = 0; k < intCount(indexArray); k++) {
                            if (indexArray[k] == (i - j * 16)) { break; }
                        }
                        if (k == intCount(indexArray)) {
                            indexArray = append(indexArray, i - j * 16);
                        }
                    }
                }
            }
            else {
                for (j = 1; j <= radius; j++) {
                    if ((board[i - j * 16] == White) || (board[i - j * 16] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i - j * 16)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i - j * 16);
                    }
                }
            }
            
            if (min(14 - (i % 15), 14 - i / 15) < radius) {
                for (j = 1; j <= radius; j++) {
                    if (j <= min(14 - (i % 15), 14 - i / 15)) {
                        if ((board[i + j * 16] == White) || (board[i + j * 16] == Black)) { continue; }
                        for (k = 0; k < intCount(indexArray); k++) {
                            if (indexArray[k] == (i + j * 16)) { break; }
                        }
                        if (k == intCount(indexArray)) {
                            indexArray = append(indexArray, i + j * 16);
                        }
                    }
                }
            }
            else {
                for (j = 1; j <= radius; j++) {
                    if ((board[i + j * 16] == White) || (board[i + j * 16] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i + j * 16)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i + j * 16);
                    }
                }
            }
            
            if (min(14 - (i % 15), i / 15) < radius) {
                for (j = 1; j <= radius; j++) {
                    if (j <= min(14 - (i % 15), i / 15)) {
                        if ((board[i - j * 14] == White) || (board[i - j * 14] == Black)) { continue; }
                        for (k = 0; k < intCount(indexArray); k++) {
                            if (indexArray[k] == (i - j * 14)) { break; }
                        }
                        if (k == intCount(indexArray)) {
                            indexArray = append(indexArray, i - j * 14);
                        }
                    }
                }
            }
            else {
                for (j = 1; j <= radius; j++) {
                    if ((board[i - j * 14] == White) || (board[i - j * 14] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i - j * 14)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i - j * 14);
                    }
                }
            }
            
            if (min(i % 15, 14 - i / 15) < radius) {
                for (j = 1; j <= radius; j++) {
                    if (j <= min(i % 15, 14 - i / 15)) {
                        if ((board[i + j * 14] == White) || (board[i + j * 14] == Black)) { continue; }
                        for (k = 0; k < intCount(indexArray); k++) {
                            if (indexArray[k] == (i + j * 14)) { break; }
                        }
                        if (k == intCount(indexArray)) {
                            indexArray = append(indexArray, i + j * 14);
                        }
                    }
                }
            }
            else {
                for (j = 1; j <= radius; j++) {
                    if ((board[i + j * 14] == White) || (board[i + j * 14] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i + j * 14)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i + j * 14);
                    }
                }
            }
        }
    }
    
    if (!intCount(indexArray)) {
        indexArray = append(indexArray, 112);
    }
    
    return indexArray;
}

int evaluateSpace(char * board, int index, int color) {
    int i, j;
    char * evaBoard = (char *)malloc(sizeof(char) * 225);
    int scoreBlack = 0, scoreWhite = 0;
    int * resultBlack = (int *)malloc(sizeof(int) * 18);
    int * resultWhite = (int *)malloc(sizeof(int) * 18);
    char * tempBoardStr = (char *)malloc(16);
    int * tempResultBlack;
    int * tempResultWhite;
    
    for (i = 0; i < 225; i++)
        evaBoard[i] = board[i];
    evaBoard[index] = color;
    resultBlack = memset(resultBlack, 0, sizeof(int) * 18);
    resultWhite = memset(resultWhite, 0, sizeof(int) * 18);
    tempBoardStr[15] = -1;
    
    //横向
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 15; j++)
            tempBoardStr[j] = evaBoard[i * 15 + j];
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
            tempBoardStr[j] = evaBoard[i + j * 15];
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
            tempBoardStr[j] = evaBoard[14 - i + j * 16];
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
            tempBoardStr[j] = evaBoard[i * 15 + j * 16];
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
            tempBoardStr[j] = evaBoard[i + j * 14];
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
            tempBoardStr[j] = evaBoard[14 + i * 15 + j * 14];
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
    
    free(evaBoard);
    free(resultBlack);
    free(resultWhite);
    free(tempBoardStr);
    return color == Black ? scoreBlack - scoreWhite : scoreWhite - scoreBlack;
}

void quickSort(int * Array, int len, int * indexArray, int descend) {
    int i = 0, j = (len - 1);
    double exchange;
    if (len > 1)
    {
        if (!descend) {
            while(i != j)
            {
                for (; j > i; j--)
                {
                    if (Array[i] > Array[j])
                    {
                        exchange = Array[j];
                        Array[j] = Array[i];
                        Array[i] = exchange;
                        exchange = indexArray[j];
                        indexArray[j] = indexArray[i];
                        indexArray[i] = exchange;
                        break;
                    }
                }
                for (; i < j; i++)
                {
                    if (Array[i] > Array[j])
                    {
                        exchange = Array[j];
                        Array[j] = Array[i];
                        Array[i] = exchange;
                        exchange = indexArray[j];
                        indexArray[j] = indexArray[i];
                        indexArray[i] = exchange;
                        break;
                    }
                }
            }
            quickSort(Array, i, indexArray, 0);
            quickSort(&Array[i + 1], len - (i + 1), &indexArray[i + 1], 0);
        }
        else {
            while(i != j)
            {
                for (; j > i; j--)
                {
                    if (Array[i] < Array[j])
                    {
                        exchange = Array[j];
                        Array[j] = Array[i];
                        Array[i] = exchange;
                        exchange = indexArray[j];
                        indexArray[j] = indexArray[i];
                        indexArray[i] = exchange;
                        break;
                    }
                }
                for (; i < j; i++)
                {
                    if (Array[i] < Array[j])
                    {
                        exchange = Array[j];
                        Array[j] = Array[i];
                        Array[i] = exchange;
                        exchange = indexArray[j];
                        indexArray[j] = indexArray[i];
                        indexArray[i] = exchange;
                        break;
                    }
                }
            }
            quickSort(Array, i, indexArray, 1);
            quickSort(&Array[i + 1], len - (i + 1), &indexArray[i + 1], 1);
        }
    }
}

int * generateCAND(char * board, int color) {
    int i;
    int * indexArray = findSpace(board);
    int indexCount = intCount(indexArray);
    int * scoreArray = (int *)malloc(sizeof(int) * indexCount);
    
    for (i = 0; i < indexCount; i++)
        scoreArray[i] = evaluateSpace(board, indexArray[i], color);
    quickSort(scoreArray, indexCount, indexArray, 1);\
    
    if (intCount(indexArray) > ChildNodes) {
        indexArray = (int *)realloc(indexArray, sizeof(int) * (ChildNodes + 1));
        indexArray[ChildNodes] = -1;
    }
    
    free(scoreArray);
    return indexArray;
}

