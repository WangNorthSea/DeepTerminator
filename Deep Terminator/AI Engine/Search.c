//
//  Search.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/14.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "settings.h"
#include "board.h"
#include "array.h"
#include "init.h"
#include "generate.h"
#include "win.h"
#include "renju.h"
#include "evaluate.h"
#include "zobrist.h"
#include "killer.h"
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/IO interface/trans.h"
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/main.h"

#ifdef Debug
int cut = 0;
#endif

struct bestLine {
    int moves;
    int indexes[Depth];
};

time_t terminateTime;
unsigned char terminate = 0;

#ifdef Ponder
int enemyIndex = 0;
unsigned char pondering = 0;
#endif

#ifdef HISTORY
int historyTable[225];
#endif

int alphaBeta(char * board, int depth, int alpha, int beta, int color, struct bestLine * bL, int maxDepth, int firstMove) {
#ifdef HASH
    int indexInHash = hashKey & hashIndex;
    if (depth < maxDepth) {
        if (zobristTable[indexInHash].key == hashKey) {
            if (zobristTable[indexInHash].depth == depth) { //depth == 0时置换表不写入，所以depth == 0时不会命中overNode以外的的局面
#ifdef Debug
                hashHit++;
#endif
                if (zobristTable[indexInHash].kind == betaNode) {
                    if (zobristTable[indexInHash].score >= beta)
                        return beta;
                }
                else if (zobristTable[indexInHash].kind == alphaNode) {
                    if (zobristTable[indexInHash].score <= alpha)
                        return alpha;
                }
                else if (zobristTable[indexInHash].kind == PVNode) {
                    if (zobristTable[indexInHash].score < beta && zobristTable[indexInHash].score > alpha)
                        return zobristTable[indexInHash].score;
                }
            }
            else if (zobristTable[indexInHash].kind == overNode && depth < maxDepth) {
#ifdef Debug
                hashHit++;
#endif
                if (zobristTable[indexInHash].score == 10000000) {
                    if  (beta == Beta)
                        return depth == 0 ? -10000000 : 10000000;
                    else
                        return depth == 0 ? -10000000 : beta;
                }
                else {
                    if (alpha == Alpha)
                        return depth == 0 ? 10000000 : -10000000;
                    else
                        return depth == 0 ? 10000000 : alpha;
                }
            }
        }
    }
#endif
    if (depth == 0) {
#ifdef Debug
        evaNodes++;
#endif
        
        if (terminateTime - time(NULL) <= 1)
            terminate = 1;
        
        bL -> moves = 0;
        if (color == Black)
            return evaluate(White);
        else
            return evaluate(Black);
    }
    
    int i;
    int score;
    int whoWin = 0;
    int tempIndex = 0;
    int foundPV = 0;
    unsigned char foundForbid = 0;
#ifdef HASH
    int maxValue = -99999999;
#endif
#ifdef HISTORY
    int bestMove = 0;
#endif
    struct bestLine bestL;
    bestL.moves = 0;
    
    for (i = 0; i < Depth; i++)
        bestL.indexes[i] = 0;
    
    int * indexArray;
    if (depth < maxDepth)
        indexArray = generateCAND(board, color, 1);
    else
        indexArray = generateCAND(board, color, 0);
    int indexCount = intCount(indexArray);
    
    if (maxDepth > IterationDepth && depth == maxDepth) {
        for (i = 0; i < indexCount; i++) {
            if (indexArray[i] == firstMove) {
                tempIndex =  indexArray[0];
                indexArray[0] = indexArray[i];
                indexArray[i] = tempIndex;
            }
        }
    }
    
#ifdef KILLER
    if (depth < maxDepth) {
        if (killer[indexInHash].key == hashKey) {
            for (i = 0; i < indexCount; i++) {
                if (indexArray[i] == killer[indexInHash].index) {
#ifdef Debug
                    killerHit++;
#endif
                    int j = i;
                    while (j > 0) {
                        tempIndex = indexArray[j];
                        indexArray[j] = indexArray[j - 1];
                        indexArray[j - 1] = tempIndex;
                        j--;
                    }
                    break;
                }
            }
        }
    }
#endif
    
    if (depth == maxDepth) {
        bL -> indexes[0] = indexArray[0];
        memcpy(bL -> indexes + 1, bestL.indexes, sizeof(int) * (bestL.moves));
        bL -> moves = bestL.moves + 1;
    }
    
    for (i = 0; i < indexCount; i++) {
        
        if (terminate)
            break;
        
        putPiece(board, indexArray[i], color);
        
#ifdef Renju
        if (color == Black) {
            if (checkForbidMove(board, indexArray[i])) {
                takePiece(board, indexArray[i], color);
                foundForbid = 1;
                continue;
            }
        }
#endif
        
        whoWin = checkWhoWin();

        if (whoWin) {
            if (whoWin == color)
                score = 10000000;
            else
                score = -10000000;
            goto someoneWin;
        }
        
        if (foundPV) {
            if (color == Black)
                score = -alphaBeta(board, depth - 1, -alpha - 1, -alpha, White, &bestL, maxDepth, firstMove);  //进行PVS
            else
                score = -alphaBeta(board, depth - 1, -alpha - 1, -alpha, Black, &bestL, maxDepth, firstMove);
            if (depth == 1)
                score = -score;
            
            if (score > alpha && score < beta) {  //PVS失败
                if (color == Black)
                    score = -alphaBeta(board, depth - 1, -beta, -alpha, White, &bestL, maxDepth, firstMove);
                else
                    score = -alphaBeta(board, depth - 1, -beta, -alpha, Black, &bestL, maxDepth, firstMove);
                if (depth == 1)
                    score = -score;
            }
        }
        else {
            if (color == Black)
                score = -alphaBeta(board, depth - 1, -beta, -alpha, White, &bestL, maxDepth, firstMove);
            else
                score = -alphaBeta(board, depth - 1, -beta, -alpha, Black, &bestL, maxDepth, firstMove);
            if (depth == 1)
                score = -score;
        }
        
    someoneWin:
        takePiece(board, indexArray[i], color);
        
        if (score == Alpha || score == Beta)
            score = alpha;
        
#ifdef Debug
#ifdef Ponder
        if (depth == maxDepth && !pondering)
            printf("%s = %d\n", transIndexToCoordinate(indexArray[i]), score);
#else
        if (depth == maxDepth)
            printf("%s = %d\n", transIndexToCoordinate(indexArray[i]), score);
#endif
#endif
        
        if (score >= beta) {
#ifdef Debug
            cut++;
#endif
#ifdef HASH
            zobristTable[indexInHash].key = hashKey;
            if (score == 10000000)
                zobristTable[indexInHash].kind = overNode;
            else
                zobristTable[indexInHash].kind = betaNode;
            zobristTable[indexInHash].depth = depth;
            zobristTable[indexInHash].score = score;
#endif
            
#ifdef KILLER
            killer[indexInHash].key = hashKey;
            killer[indexInHash].index = indexArray[i];
#endif
            
#ifdef HISTORY
            historyTable[indexArray[i]] += depth * depth;
#endif
            free(indexArray);
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            foundPV = 1;
#ifdef HISTORY
            bestMove = indexArray[i];
#endif
            bL -> indexes[0] = indexArray[i];
            memcpy(bL -> indexes + 1, bestL.indexes, sizeof(int) * (bestL.moves));
            bL -> moves = bestL.moves + 1;
        }
#ifdef HASH
        if (maxValue < score)
            maxValue = score;
#endif
    }
    
    if (indexArray[1] == -1 && foundForbid) {
        free(indexArray);
        return -10000000;
    }
    
#ifdef HASH
    if (maxValue == -10000000) {
        zobristTable[indexInHash].key = hashKey;
        zobristTable[indexInHash].kind = overNode;
        zobristTable[indexInHash].depth = 100;
        zobristTable[indexInHash].score = maxValue;
    }
    else if (foundPV) {
        zobristTable[indexInHash].key = hashKey;
        zobristTable[indexInHash].kind = PVNode;
        zobristTable[indexInHash].depth = depth;
        zobristTable[indexInHash].score = maxValue;
    }
    else {
        zobristTable[indexInHash].key = hashKey;
        zobristTable[indexInHash].kind = alphaNode;
        zobristTable[indexInHash].depth = depth;
        zobristTable[indexInHash].score = maxValue;
        
#ifdef KILLER
        if (killer[indexInHash].key == hashKey) {
            killer[indexInHash].key = 0;
            killer[indexInHash].index = 0;
        }
#endif
    }
#endif
    
#ifdef HISTORY
    if (bestMove)
        historyTable[bestMove] += depth * depth;
#endif
    free(indexArray);
    return alpha;
}

#ifdef HISTORY
void initHistoryTable(void) {
    int i;
    for (i = 0; i < 225; i++)
        historyTable[i] = 0;
}
#endif

int search(char * board, int color) {
    int i, j;
    int bestScore = 0;
    int decidedIndex = 0;
    char boardToSearch[225];
    int bests[Depth] = {0};
    struct bestLine * bL = NULL;
    
#ifdef Ponder
    terminateTime = pondering ? time(NULL) + 999 : time(NULL) + timeLimit;
#else
    terminateTime = time(NULL) + timeLimit;
#endif
    terminate = 0;
    
    for (i = 0; i < 225; i++)
        boardToSearch[i] = board[i];
    
#ifdef HISTORY
    initHistoryTable();
#endif
    for (i = IterationDepth; i <= Depth; i++) {
        for (j = 0; j < 225; j++)
            refreshed[j] = 0;
        
        bL = (struct bestLine *)malloc(sizeof(struct bestLine));
        bL -> moves = 0;
        for (j = 0; j < Depth; j++)
            bL -> indexes[j] = 0;
        if (i == IterationDepth)
            bestScore = alphaBeta(boardToSearch, i, Alpha, Beta, color, bL, i, bests[0]);
        else
            bestScore = alphaBeta(boardToSearch, i, Alpha, Beta, color, bL, i, bests[0]);
        for (j = 0; j < Depth; j++) {
            if (j == 1 && terminate)
                continue;
            bests[j] = bL -> indexes[j];
        }
        
        decidedIndex = bests[0];
        
#ifdef Debug
        if (color == Black)
            printf("Black:%s  Score:%d\n", transIndexToCoordinate(decidedIndex), bestScore);
        else
            printf("White:%s  Score:%d\n", transIndexToCoordinate(decidedIndex), bestScore);
        
        printf("bestline:");
        for (j = 0; j < Depth; j++) {
            if (!bests[j])
                break;
            printf("%s ", transIndexToCoordinate(bests[j]));
        }
        printf("\n");
#endif
        if (bestScore >= 10000000 || bestScore <= -10000000)
            break;
        
        if (terminate)
            break;
        
        if (i == 8 && terminateTime - time(NULL) <= 5)
            break;
            
        if (i < Depth) {
#ifdef Debug
            hashHit = 0;
            evaNodes = 0;
            cut = 0;
            killerHit = 0;
#endif
            free(bL);
        }
    }
    
#ifdef Ponder
    if (bests[1] && bests[0] != bests[1])
        enemyIndex = bests[1];
#endif
    
    free(bL);
    return decidedIndex;
}

#ifdef Ponder
void ponder(void) {
    int decidedIndex;
    pondering = 1;
    decidedIndex = search(board, intCount(pos) % 2 != 0 ? Black : White);   //由于之前调用putPiece函数没有改变pos，所以实际落子数为intCount(pos) + 1
    if (!pondering)
        ponderIndex = decidedIndex;
    else {
        pondering = 0;
        ponderIndex = decidedIndex;
    }
}
#endif
