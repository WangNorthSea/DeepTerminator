//
//  main.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <string.h>

extern void init(void);
extern void put(int index);
extern void removePiece(void);
extern int transCoordinateToIndex(char * position);
char board[225];
char * pos;

int main(void) {
    char input[10];
    
    init();
    while (1) {
        scanf("%s", input);
        if (!strcmp(input, "calc"))
            ;//printf("ok\n");
        else if (!strcmp(input, "exit"))
            break;//return 0;
        else if (!strcmp(input, "remove"))
            removePiece();
        else {
            put(transCoordinateToIndex(input));
        }
    }
    int i;
    for (i = 0; i < 225; i++) {
        if (i % 15 == 0) {
            printf("\n");
        }
        printf("%d", board[i]);
    }
    printf("\n");
    return 0;
}
