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

int * findSpace(char * board, unsigned char narrowed) {
    int i, j, k;
    int * indexArray = (int *)malloc(sizeof(int));
    indexArray[0] = -1;
    int scale = narrowed ? radius - 1 : radius;
    
    for (i = 0; i < 225; i++) {
        if ((board[i] == White) || (board[i] == Black)) {
            
            if (i % 15 < scale) {
                for (j = 1; j <= scale; j++) {
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
                for (j = 1; j <= scale; j++) {
                    if ((board[i - j] == White) || (board[i - j] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i - j)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i - j);
                    }
                }
            }
            
            if (14 - (i % 15) < scale) {
                for (j = 1; j <= scale; j++) {
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
                for (j = 1; j <= scale; j++) {
                    if ((board[i + j] == White) || (board[i + j] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i + j)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i + j);
                    }
                }
            }
            
            if (i / 15 < scale) {
                for (j = 1; j <= scale; j++) {
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
                for (j = 1; j <= scale; j++) {
                    if ((board[i - j * 15] == White) || (board[i - j * 15] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i - j * 15)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i - j * 15);
                    }
                }
            }
            
            if (14 - i / 15 < scale) {
                for (j = 1; j <= scale; j++) {
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
                for (j = 1; j <= scale; j++) {
                    if ((board[i + j * 15] == White) || (board[i + j * 15] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i + j * 15)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i + j * 15);
                    }
                }
            }
            
            if (min(i % 15, i / 15) < scale) {
                for (j = 1; j <= scale; j++) {
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
                for (j = 1; j <= scale; j++) {
                    if ((board[i - j * 16] == White) || (board[i - j * 16] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i - j * 16)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i - j * 16);
                    }
                }
            }
            
            if (min(14 - (i % 15), 14 - i / 15) < scale) {
                for (j = 1; j <= scale; j++) {
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
                for (j = 1; j <= scale; j++) {
                    if ((board[i + j * 16] == White) || (board[i + j * 16] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i + j * 16)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i + j * 16);
                    }
                }
            }
            
            if (min(14 - (i % 15), i / 15) < scale) {
                for (j = 1; j <= scale; j++) {
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
                for (j = 1; j <= scale; j++) {
                    if ((board[i - j * 14] == White) || (board[i - j * 14] == Black)) { continue; }
                    for (k = 0; k < intCount(indexArray); k++) {
                        if (indexArray[k] == (i - j * 14)) { break; }
                    }
                    if (k == intCount(indexArray)) {
                        indexArray = append(indexArray, i - j * 14);
                    }
                }
            }
            
            if (min(i % 15, 14 - i / 15) < scale) {
                for (j = 1; j <= scale; j++) {
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
                for (j = 1; j <= scale; j++) {
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

void newPattern(char * board, int index) {
    int pcode0 = getPatternCode(board, index, 0);
    int pcode1 = getPatternCode(board, index, 1);
    int pcode2 = getPatternCode(board, index, 2);
    int pcode3 = getPatternCode(board, index, 3);
    
    candidates[index].pat[0][patMap[pcode0 ^ (Black << 10)] & 15]++;
    candidates[index].pat[0][patMap[pcode1 ^ (Black << 10)] & 15]++;
    candidates[index].pat[0][patMap[pcode2 ^ (Black << 10)] & 15]++;
    candidates[index].pat[0][patMap[pcode3 ^ (Black << 10)] & 15]++;
    
    candidates[index].pat[1][patMap[pcode0 ^ (White << 10)] >> 4]++;
    candidates[index].pat[1][patMap[pcode1 ^ (White << 10)] >> 4]++;
    candidates[index].pat[1][patMap[pcode2 ^ (White << 10)] >> 4]++;
    candidates[index].pat[1][patMap[pcode3 ^ (White << 10)] >> 4]++;
}

void clearPattern(int pat[2][10]) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 9; j++)
            pat[i][j] = 0;
    }
}

int * refreshCandidates(char * board, unsigned char narrowed) {
    int i;
    int * indexArray = findSpace(board, narrowed);
    int spaceCount = intCount(indexArray);
    
    for (i = 0; i < spaceCount; i++) {
        if (!refreshed[indexArray[i]]) {
            refreshed[indexArray[i]] = 1;
            clearPattern(candidates[indexArray[i]].pat);
            newPattern(board, indexArray[i]);
        }
    }
    
    return indexArray;
}

int getScoreForSort(int index, int color) {
    int i;
    int selfScore = 0, enemyScore = 0;
    
    for (i = 0; i < 9; i++) {
        selfScore += candidates[index].pat[color - 1][i] * scoreForSort[i];
        enemyScore += candidates[index].pat[(color ^ 3) - 1][i] * scoreForSort[i];
    }
    
    return selfScore + enemyScore / 2;
}

int * generateCAND(char * board, int color, unsigned char narrowed) {
    int i;
    int * cands = refreshCandidates(board, narrowed);
    int spaceCount = intCount(cands);
    int * indexArray = (int *)malloc(sizeof(int));
    indexArray[0] = -1;
    
    //color - 1在pat数组内表示己方，(color ^ 3) - 1在pat数组内表示对方
restart:
    if (patCurrent.pat[color - 1][RushFour] || patCurrent.pat[color - 1][LiveFour]) {
        for (i = 0; i < spaceCount; i++) {
            if (candidates[cands[i]].pat[color - 1][Five]) {
                indexArray = append(indexArray, cands[i]);
                break;
            }
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
            if (candidates[cands[i]].pat[(color ^ 3) - 1][Five]) {
                indexArray = append(indexArray, cands[i]);
                break;
            }
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
            if (candidates[cands[i]].pat[color - 1][LiveFour]) {
                indexArray = append(indexArray, cands[i]);
                break;
            }
        }
        
        if (i == spaceCount) {
            patCurrent.pat[color - 1][LiveThree] = 0;
            patHistory[moveCount] = patCurrent;
            goto restart;
        }
    }
    else if (patCurrent.pat[(color ^ 3) - 1][LiveThree]) {
        unsigned char enemyFour = 0;
        for (i = 0; i < spaceCount; i++) {
            if (candidates[cands[i]].pat[(color ^ 3) - 1][LiveFour] || candidates[cands[i]].pat[(color ^ 3) - 1][RushFour]) {
                enemyFour++;
                indexArray = append(indexArray, cands[i]);
                continue;
            }
            
            if (candidates[cands[i]].pat[color - 1][RushFour]) {
                indexArray = append(indexArray, cands[i]);
                continue;
            }
        }
        
        if (!enemyFour) {
            patCurrent.pat[(color ^ 3) - 1][LiveThree] = 0;
            patHistory[moveCount] = patCurrent;
            indexArray[0] = -1;
            goto restart;
        }
    }
    
    if (indexArray[0] == -1) {
        int * scoreArray = (int *)malloc(sizeof(int) * spaceCount);
        for (i = 0; i < spaceCount; i++)
            scoreArray[i] = getScoreForSort(cands[i], color);
        quickSort(scoreArray, spaceCount, cands, 1);
        
        if (spaceCount > ChildNodes) {
            cands = (int *)realloc(cands, sizeof(int) * (ChildNodes + 1));
            cands[ChildNodes] = -1;
#ifdef HISTORY
            spaceCount = ChildNodes;
#endif
        }
        
#ifdef HISTORY
        scoreArray = (int *)realloc(scoreArray, sizeof(int) * spaceCount);
        for (i = 0; i < spaceCount; i++)
            scoreArray[i] = historyTable[cands[i]];
        quickSort(scoreArray, spaceCount, cands, 1);
#endif
        free(scoreArray);
        free(indexArray);
        return cands;
    }
    else {
        free(cands);
        return indexArray;
    }
}

