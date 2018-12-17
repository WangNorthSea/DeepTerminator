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
#include "board.h"
#include "settings.h"

int cut = 0;
extern int evaNodes;

int intCount(int * array);
extern int evaluate(char * board, int color);
extern int checkWhoWin(char * board);
extern int checkRenjuWhoWin(char * board);
extern int checkForbidMove(char * board);
extern int * generateCAND(char * board, int color, int firstSearch);

extern char * transIndexToCoordinate(int index);

struct bestLine {
    int moves;
    int indexes[Depth];
};

int historyTable[225] = {0};

int alphaBeta(char * board, int depth, int alpha, int beta, int color, struct bestLine * bL, int firstSearch, int maxDepth, int firstMove) {
    if (depth == 0) {
        evaNodes++;
        bL -> moves = 0;
        if (color == Black)
            return evaluate(board, White);
        else
            return evaluate(board, Black);
    }
    
    int i;
    int score;
    int whoWin = 0;
    int bestMove = 0;
    int foundPV = 0;
    int currentIndex = 0;
    //int decidedIndex = 0;
    struct bestLine bestL;
    bestL.moves = 0;
    
    for (i = 0; i < Depth; i++)
        bestL.indexes[i] = 0;
    
    int * indexArray =  generateCAND(board, color, firstSearch);
    
    int indexCount = intCount(indexArray);
    
    for (i = 0; i < indexCount; i++) {
        if (!i) {
            if (maxDepth > IterationDepth && depth == maxDepth) {
                board[firstMove] = color;
                currentIndex = firstMove;
            }
            else {
                board[indexArray[i]] = color;
                currentIndex = indexArray[i];
            }
        }
        else {
            if (maxDepth > IterationDepth && depth == maxDepth) {
                if (indexArray[i]  == firstMove)
                    continue;
                board[indexArray[i]] = color;
                currentIndex = indexArray[i];
            }
            else {
                board[indexArray[i]] = color;
                currentIndex = indexArray[i];
            }
        }
        
        if (Renju) {
            if (color == Black) {
                if (checkForbidMove(board)) {
                    board[currentIndex] = Empty;
                    continue;
                }
            }
            whoWin = checkRenjuWhoWin(board);
        }
        else
            whoWin = checkWhoWin(board);
        
        if (whoWin) {
            if (whoWin == color)
                score = 10000;
            else
                score = -10000;
            goto someoneWin;
        }
        
        if (foundPV) {
            if (color == Black)
                score = -alphaBeta(board, depth - 1, -alpha - 1, -alpha, White, &bestL, firstSearch,  maxDepth, firstMove);  //进行PVS
            else
                score = -alphaBeta(board, depth - 1, -alpha - 1, -alpha, Black, &bestL, firstSearch, maxDepth, firstMove);
            if (depth == 1)
                score = -score;
            
            if (score > alpha && score < beta) {  //PVS失败
                if (color == Black)
                    score = -alphaBeta(board, depth - 1, -beta, -alpha, White, &bestL, firstSearch, maxDepth, firstMove);
                else
                    score = -alphaBeta(board, depth - 1, -beta, -alpha, Black, &bestL, firstSearch, maxDepth, firstMove);
                if (depth == 1)
                    score = -score;
            }
        }
        else {
            if (color == Black)
                score = -alphaBeta(board, depth - 1, -beta, -alpha, White, &bestL, firstSearch, maxDepth, firstMove);
            else
                score = -alphaBeta(board, depth - 1, -beta, -alpha, Black, &bestL, firstSearch, maxDepth, firstMove);
            if (depth == 1)
                score = -score;
        }
        
    someoneWin:
        
        board[currentIndex] = Empty;
        
        if (depth == maxDepth)
            printf("%s = %d\n", transIndexToCoordinate(currentIndex), score);
        
        if (score >= beta) {
            cut++;
            historyTable[currentIndex] += depth * depth;
            free(indexArray);
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            foundPV = 1;
            bestMove = currentIndex;
            bL -> indexes[0] = currentIndex;
            memcpy(bL -> indexes + 1, bestL.indexes, sizeof(int) * (bestL.moves));
            bL -> moves = bestL.moves + 1;
            /*if (depth == Depth)
                decidedIndex = currentIndex;*/
        }
    }
    historyTable[bestMove] += depth * depth;
    
    /*if (depth == Depth)
        board[decidedIndex] = color;*/
    free(indexArray);
    return alpha;
}

void initHistoryTable(void) {
    int i;
    for (i = 0; i < 225; i++)
        historyTable[i] = 0;
}

int search(char * board, int color) {
    int i, j;
    int bestScore = 0;
    int decidedIndex = 0;
    char boardToSearch[225];
    int bests[Depth] = {0};
    struct bestLine * bL = NULL;
    
    for (i = 0; i < 225; i++)
        boardToSearch[i] = board[i];
    
    initHistoryTable();
    for (i = IterationDepth; i <= Depth; i++) {
        bL = (struct bestLine *)malloc(sizeof(struct bestLine));
        bL -> moves = 0;
        for (j = 0; j < Depth; j++)
            bL -> indexes[j] = 0;
        if (i == IterationDepth)
            bestScore = alphaBeta(boardToSearch, i, Alpha, Beta, color, bL, 1, i, decidedIndex);
        else
            bestScore = alphaBeta(boardToSearch, i, Alpha, Beta, color, bL, 0, i, decidedIndex);
        for (j = 0; j < Depth; j++)
            bests[j] = bL -> indexes[j];
        
        decidedIndex = bests[0];
        
        if (color == Black)
            printf("Black:%s  Score:%d\n", transIndexToCoordinate(decidedIndex), bestScore);
        else
            printf("White:%s  Score:%d\n", transIndexToCoordinate(decidedIndex), bestScore);
        
        printf("bestline:");
        for (j = 0; j < Depth; j++)
            printf("%s ", transIndexToCoordinate(bests[j]));
        printf("\n");
        
        if (bestScore >= 10000 || bestScore <= -10000)
            break;
            
        if (i < Depth) {
            evaNodes = 0;
            cut = 0;
            free(bL);
            /*for (j = 0; j < 225; j++)
                boardToSearch[j] = board[j];*/
        }
    }
    
    //decidedIndex = bests[0];
    
    /*if (color == Black)
        printf("Black:%s  Score:%d\n", transIndexToCoordinate(decidedIndex), bestScore);
    else
        printf("White:%s  Score:%d\n", transIndexToCoordinate(decidedIndex), bestScore);
    
    printf("bestline:");
    for (i = 0; i < Depth; i++)
        printf("%s ", transIndexToCoordinate(bests[i]));
    printf("\n");*/
    
    free(bL);
    return decidedIndex;
}
