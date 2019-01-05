//
//  Renju.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/15.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "init.h"
#include "board.h"

/*
 本函数用于检查黑棋是否触犯禁手
 */
int checkForbidMove(char * board, int index) {
    int i, count = 0;
    unsigned char pats[4];
    unsigned int pcode[4];
    pcode[0] = getPatternCode(board, index, 0);
    pcode[1] = getPatternCode(board, index, 1);
    pcode[2] = getPatternCode(board, index, 2);
    pcode[3] = getPatternCode(board, index, 3);
    
    pats[0] = patMap[pcode[0]];
    pats[1] = patMap[pcode[1]];
    pats[2] = patMap[pcode[2]];
    pats[3] = patMap[pcode[3]];
    
    for (i = 0; i < 4; i++) {
        if ((pats[i] & 15) == 5)     //判断是否同时形成双活三
            count++;
    }
    if (count >= 2)
        return 1;
    
    //双冲四特判，这里对应在同一方向形成双冲四的几种情形
    for (i = 0; i < 4; i++)
        if (pcode[i] == 70928 || pcode[i] == 595216 || pcode[i] == 70936 || pcode[i] == 595224)
            return 1;
    for (i = 0; i < 4; i++)
        if (pcode[i] == 332880 || pcode[i] == 2430032 || pcode[i] == 332888 || pcode[i] == 2430040)
            return 1;
    for (i = 0; i < 4; i++)
        if (pcode[i] == 83220 || pcode[i] == 607508 || pcode[i] == 83222 || pcode[i] == 607510)
            return 1;
    
    count = 0;
    for (i = 0; i < 4; i++) {
        if ((pats[i] & 15) == 6 || (pats[i] & 15) == 7)    //判断是否同时形成两个四
            count++;
    }
    if (count >= 2)
        return 1;
    
    return 0;
}
