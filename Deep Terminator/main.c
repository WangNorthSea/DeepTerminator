//
//  main.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//


//问题：分数区别很小、alphaBeta没有胜利截断（应该也是产生前者的原因）、落点生成还欠考虑


#include <stdio.h>
#include <string.h>
#include "board.h"

int evaNodes = 0;

extern int cut;

extern void init(void);
extern void put(int index);
extern int intCount(int * array);
extern void removePiece(void);
extern int transCoordinateToIndex(char * position);
extern char * transIndexToCoordinate(int index);

extern int search(char * board, int color);

int main(void) {
    //int i;
    int decidedIndex;
    char input[10];
    
    init();
    while (1) {
        scanf("%s", input);
        if (!strcmp(input, "calc")) {
            if (intCount(pos) % 2 != 0) {
                printf("calculating White...\n");
                decidedIndex = search(board, White);
            }
            else {
                printf("calculating Black...\n");
                decidedIndex = search(board, Black);
            }
            put(decidedIndex);
            if (intCount(pos) % 2 == 0)
                printf("White: %s\n", transIndexToCoordinate(decidedIndex));
            else
                printf("Black: %s\n", transIndexToCoordinate(decidedIndex));
            printf("evaluated nodes = %d\n", evaNodes);
            printf("alphaBeta cut = %d\n", cut);
            evaNodes = 0;
            cut = 0;
        }
        else if (!strcmp(input, "exit"))
            break;
        else if (!strcmp(input, "remove"))
            removePiece();
        else
            put(transCoordinateToIndex(input));
    }
    /*for (i = 0; i < 225; i++) {
        if (i % 15 == 0) {
            printf("\n");
        }
        printf("%d", board[i]);
    }
    printf("\n");*/
    return 0;
}
