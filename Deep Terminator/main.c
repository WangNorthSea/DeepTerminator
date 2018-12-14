//
//  main.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "board.h"

extern void init(void);
extern void put(int index);
extern void removePiece(void);
extern int transCoordinateToIndex(char * position);

extern int evaluate(char * board, int color, int nextColor);

int main(void) {
    int i;
    char input[10];
    
    init();
loop:
    while (1) {
        scanf("%s", input);
        if (!strcmp(input, "calc"))
            printf("Black score = %d\n", evaluate(board, Black, White));
        else if (!strcmp(input, "exit"))
            goto display;
        else if (!strcmp(input, "remove"))
            removePiece();
        else
            put(transCoordinateToIndex(input));
    }
display:
    for (i = 0; i < 225; i++) {
        if (i % 15 == 0) {
            printf("\n");
        }
        printf("%d", board[i]);
    }
    printf("\n");
    goto loop;
    return 0;
}
