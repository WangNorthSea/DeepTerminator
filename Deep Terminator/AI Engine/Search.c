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
extern int * generateCAND(char * board, int color);

extern char * transIndexToCoordinate(int index);

struct bestLine {
    int moves;
    int indexes[Depth];
};

int alphaBeta(char * board, int depth, int alpha, int beta, int color, struct bestLine * bL, int * bests, int firstBranch) {
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
    int first = 0;
    int currentIndex = 0;
    //int decidedIndex = 0;
    struct bestLine bestL;
    bestL.moves = 0;
    
    for (i = 0; i < Depth; i++)
        bestL.indexes[i] = 0;
    
    int * indexArray = generateCAND(board, color);
    int indexCount = intCount(indexArray);
    
    for (i = 0; i < indexCount; i++) {
        if (firstBranch && !i)
            first = 1;
        else
            first = 0;
        
        if (first && depth > 1) {
            board[bests[Depth - depth]] = color;
            currentIndex = bests[Depth - depth];
        }
        else {
            if (firstBranch) {
                if (indexArray[i] == bests[Depth - depth])
                    continue;
            }
            board[indexArray[i]] = color;
            currentIndex = indexArray[i];
        }
        
        if (Renju) {
            if (color == Black) {
                if (checkForbidMove(board)) {
                    board[indexArray[i]] = Empty;
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
        
        if (color == Black)
            score = -alphaBeta(board, depth - 1, -beta, -alpha, White, &bestL, bests, first);
        else
            score = -alphaBeta(board, depth - 1, -beta, -alpha, Black, &bestL, bests, first);
        if (depth == 1)
            score = -score;
        
    someoneWin:
        
        board[currentIndex] = Empty;
        
        if (score >= beta) {
            cut++;
            free(indexArray);
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            bL -> indexes[0] = currentIndex;
            memcpy(bL -> indexes + 1, bestL.indexes, sizeof(int) * (bestL.moves));
            bL -> moves = bestL.moves + 1;
            /*if (depth == Depth)
                decidedIndex = currentIndex;*/
        }
    }
    
    /*if (depth == Depth)
        board[decidedIndex] = color;*/
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
    
    for (i = IterationDepth; i <= Depth; i += 2) {
        bL = (struct bestLine *)malloc(sizeof(struct bestLine));
        bL -> moves = 0;
        for (j = 0; j < Depth; j++)
            bL -> indexes[j] = 0;
        if (i == IterationDepth)
            bestScore = alphaBeta(boardToSearch, i, Alpha, Beta, color, bL, bests, 0);
        else
            bestScore = alphaBeta(boardToSearch, i, Alpha, Beta, color, bL, bests, 1);
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
