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
#include "array.h"
#include "init.h"
#include "settings.h"
#include "generate.h"
#include "win.h"
#include "renju.h"
#include "evaluate.h"
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/IO interface/trans.h"
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/main.h"

int cut = 0;

struct bestLine {
    int moves;
    int indexes[Depth];
};

int alphaBeta(char * board, int depth, int alpha, int beta, int color, struct bestLine * bL, int maxDepth) {
    if (depth == 0) {
        evaNodes++;
        bL -> moves = 0;
        if (color == Black)
            return evaluate(White);
        else
            return evaluate(Black);
    }
    
    int i;
    int score;
    //int whoWin = 0;
    int foundPV = 0;
    struct bestLine bestL;
    bestL.moves = 0;
    
    for (i = 0; i < Depth; i++)
        bestL.indexes[i] = 0;
    
    int * indexArray =  generateCAND(board);
    int indexCount = intCount(indexArray);
    
    for (i = 0; i < indexCount; i++) {
        putPiece(board, indexArray[i], color);
        
        /*if (Renju) {
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
                score = 10000000;
            else
                score = -10000000;
            goto someoneWin;
        }*/
        
        if (foundPV) {
            if (color == Black)
                score = -alphaBeta(board, depth - 1, -alpha - 1, -alpha, White, &bestL, maxDepth);  //进行PVS
            else
                score = -alphaBeta(board, depth - 1, -alpha - 1, -alpha, Black, &bestL, maxDepth);
            if (depth == 1)
                score = -score;
            
            if (score > alpha && score < beta) {  //PVS失败
                if (color == Black)
                    score = -alphaBeta(board, depth - 1, -beta, -alpha, White, &bestL, maxDepth);
                else
                    score = -alphaBeta(board, depth - 1, -beta, -alpha, Black, &bestL, maxDepth);
                if (depth == 1)
                    score = -score;
            }
        }
        else {
            if (color == Black)
                score = -alphaBeta(board, depth - 1, -beta, -alpha, White, &bestL, maxDepth);
            else
                score = -alphaBeta(board, depth - 1, -beta, -alpha, Black, &bestL, maxDepth);
            if (depth == 1)
                score = -score;
        }
        
    someoneWin:
        takePiece(board, indexArray[i]);
        
        if (depth == maxDepth)
            printf("%s = %d\n", transIndexToCoordinate(indexArray[i]), score);
        
        if (score >= beta) {
            cut++;
            free(indexArray);
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            foundPV = 1;
            bL -> indexes[0] = indexArray[i];
            memcpy(bL -> indexes + 1, bestL.indexes, sizeof(int) * (bestL.moves));
            bL -> moves = bestL.moves + 1;
        }
    }
    
    free(indexArray);
    return alpha;
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
    
    for (i = IterationDepth; i <= Depth; i++) {
        bL = (struct bestLine *)malloc(sizeof(struct bestLine));
        bL -> moves = 0;
        for (j = 0; j < Depth; j++)
            bL -> indexes[j] = 0;
        if (i == IterationDepth)
            bestScore = alphaBeta(boardToSearch, i, Alpha, Beta, color, bL, i);
        else
            bestScore = alphaBeta(boardToSearch, i, Alpha, Beta, color, bL, i);
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
        }
    }
    
    free(bL);
    return decidedIndex;
}
