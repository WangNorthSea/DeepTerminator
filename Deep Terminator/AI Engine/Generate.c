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

struct node * rootBlack;
struct node * rootWhite;

extern int historyTable[225];

extern int intCount(int * array);
extern int * append(int * array, int value);
int getScore(int * result);
char *** getBoardStr(char * board);
extern int * recognize(char * pattern, struct node * root, int keycharCount);

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

int evaluateSpace(char * board, int color) {
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
    
    scoreBlack = getScore(resultBlack);
    scoreWhite = getScore(resultWhite);
    
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

int * generateCAND(char * board, int color, int firstSearch) {
    int i;
    int * indexArray = findSpace(board);
    
    if (EnableHistoryTable && !firstSearch) {
        int indexCount = intCount(indexArray);
        int * scoreArray = (int *)malloc(sizeof(int) * indexCount);
    
        for (i = 0; i < indexCount; i++)
            scoreArray[i] = historyTable[indexArray[i]];
        quickSort(scoreArray, indexCount, indexArray, 1);
        
        free(scoreArray);
    }
    
    
    /*char evaBoard[225];
    
    for (i = 0; i < 225; i++)
        evaBoard[i] = board[i];
    
    for (i = 0; i < indexCount; i++) {
        evaBoard[indexArray[i]] = color;
        scoreArray[i] = evaluateSpace(evaBoard, color);
        evaBoard[indexArray[i]] = Empty;
    }
    quickSort(scoreArray, indexCount, indexArray, 1);*/
    
    if (!firstSearch) {
        if (intCount(indexArray) > ChildNodes) {
            indexArray = (int *)realloc(indexArray, sizeof(int) * (ChildNodes + 1));
            indexArray[ChildNodes] = -1;
        }
    }
    
    return indexArray;
}

