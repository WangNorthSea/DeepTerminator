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

void initACautomaton(void) {
    int i;
    rootBlack = (struct node *)malloc(sizeof(struct node));
    rootWhite = (struct node *)malloc(sizeof(struct node));
    rootBlack -> next[0] = NULL;
    rootBlack -> next[1] = NULL;
    rootBlack -> next[2] = NULL;
    rootBlack -> fail = NULL;
    rootBlack -> id = -1;
    rootWhite -> next[0] = NULL;
    rootWhite -> next[1] = NULL;
    rootWhite -> next[2] = NULL;
    rootWhite -> fail = NULL;
    rootWhite -> id = -1;
    
    for (i = 0; i < 18; i++)
        insert(blackPatterns[i], rootBlack, i);
    for (i = 0; i < 18; i++)
        insert(whitePatterns[i], rootWhite, i);
    buildFailPtr(rootBlack);
    buildFailPtr(rootWhite);
}

void init(void) {
    int i;
    for (i = 0; i < 225; i++)
        board[i] = Empty;
    pos = (int *)malloc(sizeof(int));
    pos[0] = -1;
    initACautomaton();
}
