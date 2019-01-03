//
//  CLI.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2019/1/3.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/IO interface/trans.h"
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/AI Engine/board.h"
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/AI Engine/array.h"

wchar_t * genBoard(void) {
    int i;
    int pieceCount = intCount(pos);
    wchar_t * boardString = (wchar_t *)malloc(sizeof(wchar_t) * 225);
    
    for (i = 0; i < 225; i++) {
        if (i == 0) { boardString[i] = L'┌'; }
        else if (i == 14) { boardString[i] = L'┐'; }
        else if (i == 210) { boardString[i] = L'└'; }
        else if (i == 224) { boardString[i] = L'┘'; }
        else if ((i > 0) && (i < 14)) { boardString[i] = L'┬'; }
        else if ((i > 210) && (i < 224)) { boardString[i] = L'┴'; }
        else if (i % 15 == 0) { boardString[i] = L'├'; }
        else if ((i + 1) % 15 == 0) { boardString[i] = L'┤'; }
        else { boardString[i] = L'┼'; }
    }
    
    for (i = 0; i < pieceCount; i++) {
        if (i == pieceCount - 1) {
            if (pieceCount % 2 != 0)
                boardString[pos[i]] = L'▲';
            else
                boardString[pos[i]] = L'△';
            break;
        }
        
        if (i % 2 != 0)
            boardString[pos[i]] = L'○';
        else
            boardString[pos[i]] = L'●';
    }
    
    return boardString;
}

void printBoard(void) {
    system("clear");
    int i, j;
    int pieceCount = intCount(pos);
    wchar_t * boardString = genBoard();
    
    printf("\nWelcome to Renju Game!\n\n");
    for (i = 15; i > 0; i--) {
        printf("%2d  ", i);
        for (j = (15 - i) * 15; j < (15 - i) * 15 + 15; j++) {
            printf("%lc", boardString[j]);
        }
        printf("\n");
    }
    
    printf("    A B C D E F G H I J K L M N O\n\n");
    if (pieceCount % 2 != 0) {
        printf("Black:%s\n", transIndexToCoordinate(pos[pieceCount - 1]));
        printf("Please enter a position like 'h7', now you are the white:");
    }
    else {
        if (pieceCount)
            printf("White:%s\n", transIndexToCoordinate(pos[pieceCount - 1]));
        printf("Please enter a position like 'h7', now you are the black:");
    }
    
    free(boardString);
}
