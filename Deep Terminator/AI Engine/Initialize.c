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

unsigned char patMap[1 << 22];

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
    int i, j;
    for (i = 0; i < 225; i++)
        board[i] = Empty;
    pos = (int *)malloc(sizeof(int));
    pos[0] = -1;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 10; j++)
            patCurrent.pat[i][j] = 0;
    }
    struct pattern initialPat;
    initialPat.pat[0][0] = 0;    //这句只是为了消除initialPat未初始化的警告
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 10; j++)
            initialPat.pat[i][j] = 0;
    }
    patHistory[0] = initialPat;
    //initACautomaton();
    initPatMap();
}
