//
//  CLI.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2019/1/3.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "../IOinterface/trans.h"
#include "../AIEngine/board.h"
#include "../AIEngine/array.h"

/*
 本函数用于生成可以被打印出来的棋盘数组
 */
wchar_t * genBoard(void) {
    int i;
    int pieceCount = intCount(pos);
    wchar_t * boardString = (wchar_t *)malloc(sizeof(wchar_t) * 225);
    
    for (i = 0; i < 225; i++) {      //先填充上棋盘符号
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
    
    for (i = 0; i < pieceCount; i++) {        //将对应位置的字符换成棋子
        if (i == pieceCount - 1) {
            if (pieceCount % 2 != 0)
                boardString[pos[i]] = L'▲';   //最后一个落子用三角形字符
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

/*
 本函数用于打印棋盘
 */
void printBoard(void) {
    system("clear");         //清屏命令
    int i, j;
    int pieceCount = intCount(pos);
    wchar_t * boardString = genBoard();    //生成用于打印的棋盘数组
    
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
        printf("Black:%s\n", transIndexToCoordinate(pos[pieceCount - 1]));       //黑棋最后落子
        printf("Please enter a position like 'h7', now you are the white:");
    }
    else {
        if (pieceCount)  //还没有落子时不执行此分支
            printf("White:%s\n", transIndexToCoordinate(pos[pieceCount - 1]));   //白棋最后落子
        printf("Please enter a position like 'h7', now you are the black:");
    }
    
    free(boardString);
}
