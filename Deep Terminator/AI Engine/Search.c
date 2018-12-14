//
//  Search.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/14.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "settings.h"

int cut = 0;
extern int evaNodes;

int intCount(int * array);
extern int evaluate(char * board, int color, int nextColor);
extern int * generateCAND(char * board, int color);

extern char * transIndexToCoordinate(int index);


int alphaBeta(char * board, int depth, int alpha, int beta, int color) {
    if (depth == 0) {
        evaNodes++;
        if (color == Black)
            return evaluate(board, White, Black);
        else
            return evaluate(board, Black, White);
    }
    
    int i;
    int score;
    int decidedIndex = 0;
    int * indexArray = generateCAND(board, color);
    int indexCount = intCount(indexArray);
    
    for (i = 0; i < indexCount; i++) {
        board[indexArray[i]] = color;
        
        if (color == Black)
            score = -alphaBeta(board, depth - 1, -beta, -alpha, White);
        else
            score = -alphaBeta(board, depth - 1, -beta, -alpha, Black);
        
        board[indexArray[i]] = Empty;
        
        if (depth == Depth) {
            printf("%s = %d\n", transIndexToCoordinate(indexArray[i]), score);
        }
        
        if (score >= beta) {
            cut++;
            return beta;
        }
        if (score > alpha) {
            alpha = score;
            if (depth == Depth)
                decidedIndex = indexArray[i];
        }
    }
    
    if (depth == Depth)
        board[decidedIndex] = color;
    return alpha;
}

int search(char * board, int color) {
    int i;
    int bestScore;
    int decidedIndex = 0;
    char * boardToSearch = (char *)malloc(sizeof(char) * 225);
    
    for (i = 0; i < 225; i++)
        boardToSearch[i] = board[i];
    
    bestScore = alphaBeta(boardToSearch, Depth, Alpha, Beta, color);
    
    for (i = 0; i < 225; i++) {
        if (board[i] != boardToSearch[i]) {
            decidedIndex = i;
            break;
        }
    }
    
    free(boardToSearch);
    return decidedIndex;
}
