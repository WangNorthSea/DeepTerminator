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

int scoreForSort[9] = {   //存储生成落点时各种棋型的分数，用于落点排序
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

/*
 本函数用于针对某一张棋盘，找出所有在一定的半径内有棋子的空位
 */
int * findSpace(char * board, unsigned char narrowed) {
    int i, j, k;
    int * indexArray = (int *)malloc(sizeof(int));
    indexArray[0] = -1;
    int scale = narrowed ? radius - 1 : radius;   //如果narrowed为0，则不缩小半径，否则将半径缩小为radius - 1
    
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
    
    if (!intCount(indexArray)) {     //如果棋盘上没有棋子，则选择空位h8
        indexArray = append(indexArray, 112);
    }
    
    return indexArray;
}

/*
 本函数用于对数组进行快速排序，Array为作为排序依据的数组，len为数组长度，indexArray随着Array中元素位置变化而作相同的变化，其与Array长度相同，descend为1代表进行降序排序，为0进行升序排序
 */
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

/*
 本函数用于更新某个落点的双方棋型
 */
void newPattern(char * board, int index) {
    int pcode0 = getPatternCode(board, index, 0);   //获取四个方向的棋盘二进制编码
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

/*
 本函数用于清空某个落点的双方棋型
 */
void clearPattern(int pat[2][10]) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 9; j++)
            pat[i][j] = 0;
    }
}

/*
 本函数用于生成并更新落点的双方棋型，先获取空位，再更新被标记为需要更新的落点的双方棋型
 */
int * refreshCandidates(char * board, unsigned char narrowed) {
    int i;
    int * indexArray = findSpace(board, narrowed);    //获取空位
    int spaceCount = intCount(indexArray);
    
    for (i = 0; i < spaceCount; i++) {
        if (!refreshed[indexArray[i]]) {    //如果某个落点被标记为需要更新，即refreshed[indexArray[i]] == 0
            refreshed[indexArray[i]] = 1;   //先标记为已更新
            clearPattern(candidates[indexArray[i]].pat);    //清空现有棋型
            newPattern(board, indexArray[i]);    //获取新棋型
        }
    }
    
    return indexArray;
}

/*
 本函数用于对落点进行评分，从而使得可以对落点进行排序
 */
int getScoreForSort(int index, int color) {
    int i;
    int selfScore = 0, enemyScore = 0;
    
    for (i = 0; i < 9; i++) {
        selfScore += candidates[index].pat[color - 1][i] * scoreForSort[i];
        enemyScore += candidates[index].pat[(color ^ 3) - 1][i] * scoreForSort[i];
    }
    
    return selfScore + enemyScore / 2;
}

/*
 本函数用于在搜索过程中生成落点
 */
int * generateCAND(char * board, int color, unsigned char narrowed) {
    int i;
    int * cands = refreshCandidates(board, narrowed);   //获取空位，并更新棋型
    int spaceCount = intCount(cands);
    int * indexArray = (int *)malloc(sizeof(int));
    indexArray[0] = -1;
    
    //color - 1在pat数组内表示己方，(color ^ 3) - 1在pat数组内表示对方
restart:
    if (patCurrent.pat[color - 1][RushFour] || patCurrent.pat[color - 1][LiveFour]) {   //如果己方有四，则寻找成五点
        for (i = 0; i < spaceCount; i++) {
            if (candidates[cands[i]].pat[color - 1][Five]) {
                indexArray = append(indexArray, cands[i]);   //找到一个就可以
                break;
            }
        }
        
        if (i == spaceCount) {   //没有找到则说明棋型信息有误，进行更正并重新开始
            patCurrent.pat[color - 1][RushFour] = 0;
            patCurrent.pat[color - 1][LiveFour] = 0;
            patHistory[moveCount] = patCurrent;
            goto restart;
        }
    }
    else if (patCurrent.pat[(color ^ 3) - 1][RushFour] || patCurrent.pat[(color ^ 3) - 1][LiveFour]) {  //如果己方无四时对方有四，则寻找对方成五点
        for (i = 0; i < spaceCount; i++) {
            if (candidates[cands[i]].pat[(color ^ 3) - 1][Five]) {
                indexArray = append(indexArray, cands[i]);   //找到一个就可以
                break;
            }
        }
        
        if (i == spaceCount) {   //没有找到则说明棋型信息有误，进行更正并重新开始
            patCurrent.pat[(color ^ 3) - 1][RushFour] = 0;
            patCurrent.pat[(color ^ 3) - 1][LiveFour] = 0;
            patHistory[moveCount] = patCurrent;
            goto restart;
        }
    }
    else if (patCurrent.pat[color - 1][LiveThree]) {     //如果双方都没有四时己方有活三，则寻找己方活四点
        for (i = 0; i < spaceCount; i++) {
            if (candidates[cands[i]].pat[color - 1][LiveFour]) {
                indexArray = append(indexArray, cands[i]);    //找到全部活四点，正常找到一个就可以，主要是考虑到禁手的问题
                continue;
            }
        }
        
        if (i == spaceCount) {   //没有找到则说明棋型信息有误，进行更正并重新开始
            patCurrent.pat[color - 1][LiveThree] = 0;
            patHistory[moveCount] = patCurrent;
            goto restart;
        }
    }
    else if (patCurrent.pat[(color ^ 3) - 1][LiveThree]) {   //如果双方都没有四且己方没有活三时对方有活三，则寻找对方成活四或冲四以及己方成冲四的点
        unsigned char enemyFour = 0;
        for (i = 0; i < spaceCount; i++) {
            if (candidates[cands[i]].pat[(color ^ 3) - 1][LiveFour] || candidates[cands[i]].pat[(color ^ 3) - 1][RushFour]) {
                enemyFour++;
                indexArray = append(indexArray, cands[i]);    //找到对方全部活四或冲四点
                continue;
            }
            
            if (candidates[cands[i]].pat[color - 1][RushFour]) {   //找到己方全部冲四点
                indexArray = append(indexArray, cands[i]);
                continue;
            }
        }
        
        if (!enemyFour) {     //没有找到对方活四或冲四点则说明棋型信息有误，进行更正并重新开始
            patCurrent.pat[(color ^ 3) - 1][LiveThree] = 0;
            patHistory[moveCount] = patCurrent;
            indexArray[0] = -1;
            goto restart;
        }
    }
    
    if (indexArray[0] == -1) {     //如果双方都没有活三及以上的棋型
        int * scoreArray = (int *)malloc(sizeof(int) * spaceCount);
        for (i = 0; i < spaceCount; i++)
            scoreArray[i] = getScoreForSort(cands[i], color);
        quickSort(scoreArray, spaceCount, cands, 1);    //对空位进行评分并排序
        
        if (spaceCount > ChildNodes) {     //将排序后的空位进行有害裁剪，只选取前ChildNodes个空位
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
        quickSort(scoreArray, spaceCount, cands, 1);   //根据历史表中的分数，对空位再进行一次排序
#endif
        free(scoreArray);
        free(indexArray);
        return cands;
    }
    else {    //如果双方有活三及以上的棋型，则返回寻找到的对应落点
        free(cands);
        return indexArray;
    }
}

