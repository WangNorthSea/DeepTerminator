//
//  Initialize.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "board.h"
#include "ACautomaton.h"

extern void insert(char * str, struct node * root, int id);
extern void buildFailPtr(struct node * root);

struct node * rootBlack;
struct node * rootWhite;
struct node * rootBlackWin;
struct node * rootWhiteWin;

char board[225];
int * pos;

char blackPatterns[18][10] = {
    {Black, Black, Black, Black, Black, Stop},           //Consecutive Five
    {Empty, Black, Black, Black, Black, Empty, Stop},        //Live Four
    {Black, Black, Black, Empty, Black, Stop},           //Rush Four
    {Black, Empty, Black, Black, Black, Stop},
    {Empty, Black, Black, Black, Black, White, Stop},
    {White, Black, Black, Black, Black, Empty, Stop},
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

char whitePatterns[18][10] = {
    {White, White, White, White, White, Stop},
    {Empty, White, White, White, White, Empty, Stop},
    {White, White, White, Empty, White, Stop},
    {White, Empty, White, White, White, Stop},
    {Empty, White, White, White, White, Black, Stop},
    {Black, White, White, White, White, Empty, Stop},
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
    
    initRoot(rootBlack);
    initRoot(rootWhite);
    initRoot(rootBlackWin);
    initRoot(rootWhiteWin);
    
    for (i = 0; i < 18; i++)
        insert(blackPatterns[i], rootBlack, i);
    for (i = 0; i < 18; i++)
        insert(whitePatterns[i], rootWhite, i);
    insert(blackWin, rootBlackWin, 0);
    insert(whiteWin, rootWhiteWin, 0);
    
    buildFailPtr(rootBlack);
    buildFailPtr(rootWhite);
    buildFailPtr(rootBlackWin);
    buildFailPtr(rootWhiteWin);
}

void init(void) {
    int i;
    for (i = 0; i < 225; i++)
        board[i] = Empty;
    pos = (int *)malloc(sizeof(int));
    pos[0] = -1;
    initACautomaton();
}
