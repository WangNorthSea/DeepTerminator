//
//  Initialize.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "board.h"
#include "settings.h"
#include "patterns.h"
#include "AC.h"

struct node * rootBlack;
struct node * rootWhite;
struct node * rootBlackWin;
struct node * rootWhiteWin;
struct node * rootBlackRenjuWin;
struct node * rootForbidMove;

unsigned char patMap[1 << 22];

char blackPatterns[19][10] = {
    {Black, Black, Black, Black, Black, Stop},           //Consecutive Five
    {Empty, Black, Black, Black, Black, Empty, Stop},        //Live Four
    {Black, Black, Black, Empty, Black, Stop},           //Rush Four
    {Black, Empty, Black, Black, Black, Stop},
    {Empty, Black, Black, Black, Black, White, Stop},
    {White, Black, Black, Black, Black, Empty, Stop},
    {Black, Black, Empty, Black, Black, Stop},
    {Empty, Black, Black, Black, Empty, Stop},           //Live Three
    {Empty, Black, Black, Empty, Black, Empty, Stop},
    {Empty, Black, Empty, Black, Black, Empty, Stop},
    {Empty, Black, Black, Black, White, Stop},           //Sleep Three
    {White, Black, Black, Black, Empty, Stop},
    {White, Black, Black, Empty, Black, Empty, Stop},
    {Empty, Black, Black, Empty, Black, White, Stop},
    {White, Black, Empty, Black, Black, Empty, Stop},
    {Empty, Black, Empty, Black, Black, White, Stop},
    {Empty, Black, Black, Empty, Stop},              //Live Two
    {Empty, Black, Empty, Black, Empty, Stop},
    {Empty, Black, Empty, Empty, Black, Empty, Stop}
};

char whitePatterns[19][10] = {
    {White, White, White, White, White, Stop},
    {Empty, White, White, White, White, Empty, Stop},
    {White, White, White, Empty, White, Stop},
    {White, Empty, White, White, White, Stop},
    {Empty, White, White, White, White, Black, Stop},
    {Black, White, White, White, White, Empty, Stop},
    {White, White, Empty, White, White, Stop},
    {Empty, White, White, White, Empty, Stop},
    {Empty, White, White, Empty, White, Empty, Stop},
    {Empty, White, Empty, White, White, Empty, Stop},
    {Empty, White, White, White, Black, Stop},
    {Black, White, White, White, Empty, Stop},
    {Black, White, White, Empty, White, Empty, Stop},
    {Empty, White, White, Empty, White, Black, Stop},
    {Black, White, Empty, White, White, Empty, Stop},
    {Empty, White, Empty, White, White, Black, Stop},
    {Empty, White, White, Empty, Stop},
    {Empty, White, Empty, White, Empty, Stop},
    {Empty, White, Empty, Empty, White, Empty, Stop}
};

char blackWin[10] = {Black, Black, Black, Black, Black, Stop};
char whiteWin[10] = {White, White, White, White, White, Stop};

char blackRenjuWin[4][10] = {
    {Empty, Black, Black, Black, Black, Black, Empty, Stop},
    {White, Black, Black, Black, Black, Black, Empty, Stop},
    {Empty, Black, Black, Black, Black, Black, White, Stop},
    {White, Black, Black, Black, Black, Black, White, Stop}
};

char forbidPatterns[9][10] = {
    {Black, Black, Black, Black, Black, Black, Stop},    //Consecutive Six
    {Empty, Black, Black, Black, Black, Empty, Stop},    //Live Four
    {Black, Black, Black, Empty, Black, Stop},           //Rush Four
    {Black, Empty, Black, Black, Black, Stop},
    {Empty, Black, Black, Black, Black, White, Stop},
    {White, Black, Black, Black, Black, Empty, Stop},
    {Empty, Black, Black, Black, Empty, Stop},           //Live Three
    {Empty, Black, Black, Empty, Black, Empty, Stop},
    {Empty, Black, Empty, Black, Black, Empty, Stop},
};

void initRoot(struct node * root) {
    root -> next[0] = NULL;
    root -> next[1] = NULL;
    root -> next[2] = NULL;
    root -> fail = NULL;
    root -> id = -1;
}

void initACautomaton(void) {
    int i;
    rootBlack = (struct node *)malloc(sizeof(struct node));
    rootWhite = (struct node *)malloc(sizeof(struct node));
    rootBlackWin = (struct node *)malloc(sizeof(struct node));
    rootWhiteWin = (struct node *)malloc(sizeof(struct node));
    
    if (Renju) {
        rootBlackRenjuWin = (struct node *)malloc(sizeof(struct node));
        rootForbidMove = (struct node *)malloc(sizeof(struct node));
        initRoot(rootBlackRenjuWin);
        initRoot(rootForbidMove);
        
        for (i = 0; i < 4; i++)
            insert(blackRenjuWin[i], rootBlackRenjuWin, i);
        for (i = 0; i < 9; i++)
            insert(forbidPatterns[i], rootForbidMove, i);
        
        buildFailPtr(rootBlackRenjuWin);
        buildFailPtr(rootForbidMove);
    }
    
    initRoot(rootBlack);
    initRoot(rootWhite);
    initRoot(rootBlackWin);
    initRoot(rootWhiteWin);
    
    for (i = 0; i < 19; i++)
        insert(blackPatterns[i], rootBlack, i);
    for (i = 0; i < 19; i++)
        insert(whitePatterns[i], rootWhite, i);
    insert(blackWin, rootBlackWin, 0);
    insert(whiteWin, rootWhiteWin, 0);
    
    buildFailPtr(rootBlack);
    buildFailPtr(rootWhite);
    buildFailPtr(rootBlackWin);
    buildFailPtr(rootWhiteWin);
}

void initPatMap(void) {
    int i;
    unsigned char line[11] = {0};
    for (i = 0; i < (1 << 22); i++) {
        int temp = i;
        int j;
        
        for (j = 10; j >= 0; j--) {
            line[j] = temp % 4;
            temp /= 4;
        }
        
        int len, space;
        unsigned short int key = 1;
        unsigned char pattern = 0;
        unsigned char * left = line + 5, * right = line + 5;
        
        for (len = 0, space = 0; left >= line && len < 6 && *left != White && *left != Extra && space < 4; len++, left--) {
            key = key << 1;
            if (*left == Black)
                key++;
            
            if (*left == Empty)
                space++;
            else
                space = 0;
        }
        key = patternIndex[key];
        
        for (len = 0, space = 0; right < line + 11 && len < 6 && *right != White && *right != Extra && space < 4; len++, right++) {
            key = key << 1;
            if (*right == Black)
                key++;
            
            if (*right == Empty)
                space++;
            else
                space = 0;
        }
        pattern += patternMap[key];
        
        key = 1;
        left = line + 5;
        right = line + 5;
        
        for (len = 0, space = 0; left >= line && len < 6 && *left != Black && *left != Extra && space < 4; len++, left--) {
            key = key << 1;
            if (*left == White)
                key++;
            
            if (*left == Empty)
                space++;
            else
                space = 0;
        }
        key = patternIndex[key];
        
        for (len = 0, space = 0; right < line + 11 && *right != Black && *right != Extra && len < 6 && space < 4; len++, right++) {
            key = key << 1;
            if (*right == White)
                key++;
            
            if (*right == Empty)
                space++;
            else
                space = 0;
        }
        pattern += patternMap[key] << 4;
        patMap[i] = pattern;
    }
}

void init(void) {
    int i;
    for (i = 0; i < 225; i++)
        board[i] = Empty;
    pos = (int *)malloc(sizeof(int));
    pos[0] = -1;
    //initACautomaton();
    initPatMap();
}
