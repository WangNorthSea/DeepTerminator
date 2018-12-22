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
#include "array.h"
#include "init.h"
#include "evaluate.h"
#include "search.h"
#include "patterns.h"

int scoreForSort[9] = {
    1,
    4,
    4,
    16,
    16,
    64,
    64,
    256,
    10000
};

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

int * newPattern(char * board, int index, int color) {
    int i;
    int * newPats = (int *)malloc(sizeof(int) * 10);
    memset(newPats, 0, sizeof(int) * 10);
    
    if (color == Black) {
        for (i = 0; i < 4; i++)
            newPats[patMap[getPatternCode(board, index, i) ^ (Black << 10)] & 15]++;
    }
    else {
        for (i = 0; i < 4; i++)
            newPats[patMap[getPatternCode(board, index, i) ^ (White << 10)] >> 4]++;
    }
    
    return newPats;
}

int getScoreForSort(int * patterns) {
    int i;
    int score = 0;
    
    for (i = 0; i < 9; i++)
        score += patterns[i] * scoreForSort[i];
    
    return score;
}

int * generateCAND(char * board, int color) {
    int i;
    int * spaceArray = findSpace(board);
    int spaceCount = intCount(spaceArray);
    int * newPats;
    int * indexArray = (int *)malloc(sizeof(int));
    indexArray[0] = -1;
    
    //color - 1在pat数组内表示己方，(color ^ 3) - 1在pat数组内表示对方
restart:
    if (patCurrent.pat[color - 1][RushFour] || patCurrent.pat[color - 1][LiveFour]) {
        for (i = 0; i < spaceCount; i++) {
            newPats = newPattern(board, spaceArray[i], color);
            if (newPats[Five]) {
                indexArray = append(indexArray, spaceArray[i]);
                free(newPats);
                break;
            }
            free(newPats);
        }
        
        if (i == spaceCount) {
            patCurrent.pat[color - 1][RushFour] = 0;
            patCurrent.pat[color - 1][LiveFour] = 0;
            patHistory[moveCount] = patCurrent;
            goto restart;
        }
    }
    else if (patCurrent.pat[(color ^ 3) - 1][RushFour] || patCurrent.pat[(color ^ 3) - 1][LiveFour]) {
        for (i = 0; i < spaceCount; i++) {
            newPats = newPattern(board, spaceArray[i], color ^ 3);
            if (newPats[Five]) {
                indexArray = append(indexArray, spaceArray[i]);
                free(newPats);
                break;
            }
            free(newPats);
        }
        
        if (i == spaceCount) {
            patCurrent.pat[(color ^ 3) - 1][RushFour] = 0;
            patCurrent.pat[(color ^ 3) - 1][LiveFour] = 0;
            patHistory[moveCount] = patCurrent;
            goto restart;
        }
    }
    else if (patCurrent.pat[color - 1][LiveThree]) {
        for (i = 0; i < spaceCount; i++) {
            newPats = newPattern(board, spaceArray[i], color);
            if (newPats[LiveFour]) {
                indexArray = append(indexArray, spaceArray[i]);
                free(newPats);
                break;
            }
            free(newPats);
        }
        
        if (i == spaceCount) {
            patCurrent.pat[color - 1][LiveThree] = 0;
            patHistory[moveCount] = patCurrent;
            goto restart;
        }
    }
    else if (patCurrent.pat[(color ^ 3) - 1][LiveThree]) {
        unsigned char enemyLiveFour = 0;
        for (i = 0; i < spaceCount; i++) {
            newPats = newPattern(board, spaceArray[i], color ^ 3);
            if (newPats[LiveFour]) {
                enemyLiveFour++;
                indexArray = append(indexArray, spaceArray[i]);
                free(newPats);
                continue;
            }
            free(newPats);
            
            newPats = newPattern(board, spaceArray[i], color);
            if (newPats[RushFour]) {
                indexArray = append(indexArray, spaceArray[i]);
                free(newPats);
                continue;
            }
            free(newPats);
        }
        
        if (!enemyLiveFour) {
            patCurrent.pat[(color ^ 3) - 1][LiveThree] = 0;
            patHistory[moveCount] = patCurrent;
            indexArray[0] = -1;
            goto restart;
        }
    }
    
    if (indexArray[0] == -1) {
        int * scoreArray = (int *)malloc(sizeof(int) * spaceCount);
        for (i = 0; i < spaceCount; i++) {
            newPats = newPattern(board, spaceArray[i], color);
            scoreArray[i] = getScoreForSort(newPats);
            free(newPats);
        }
        quickSort(scoreArray, spaceCount, spaceArray, 1);
        
        if (spaceCount > ChildNodes) {
            spaceArray = (int *)realloc(spaceArray, sizeof(int) * (ChildNodes + 1));
            spaceArray[ChildNodes] = -1;
#ifdef HISTORY
            spaceCount = ChildNodes;
#endif
        }
        
#ifdef HISTORY
        scoreArray = (int *)realloc(scoreArray, sizeof(int) * spaceCount);
        for (i = 0; i < spaceCount; i++)
            scoreArray[i] = historyTable[spaceArray[i]];
        quickSort(scoreArray, spaceCount, spaceArray, 1);
#endif
        free(scoreArray);
        free(indexArray);
        return spaceArray;
    }
    else {
        free(spaceArray);
        return indexArray;
    }
}

