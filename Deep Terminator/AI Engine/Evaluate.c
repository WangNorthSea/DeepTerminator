//
//  Evaluate.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/14.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include <memory.h>
#include "board.h"
#include "init.h"
#include "patterns.h"

int patternScore[9] = {
    3,            //眠一
    15,           //活一
    45,           //眠二
    180,          //活二
    280,          //眠三
    485,          //活三
    580,          //眠四
    850,          //活四
    10000000      //连五
};

int getScore(int * patterns) {
    int i;
    int score = 0;
    
    for (i = 0; i < 9; i++)
        score += patterns[i] * patternScore[i];
    
    return score;
}

int evaluate(int color) {
    if (patCurrent.pat[(color ^ 3) - 1][RushFour] || patCurrent.pat[(color ^ 3) - 1][LiveFour])
        return -10000000;
    else if (patCurrent.pat[color - 1][RushFour] + patCurrent.pat[color - 1][LiveFour] >= 2)
        return 10000000;
    else if (!patCurrent.pat[color - 1][RushFour] && !patCurrent.pat[color - 1][LiveFour] && patCurrent.pat[(color ^ 3) - 1][LiveThree])
        return -10000000;
    
    int scoreBlack = getScore(patCurrent.pat[0]);
    int scoreWhite = getScore(patCurrent.pat[1]);
    return color == Black ? scoreBlack - scoreWhite : scoreWhite - scoreBlack;
}
