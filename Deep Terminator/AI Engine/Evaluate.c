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

int patternScore[9] = {     //存储每一种棋型在评估局面时对应的分数
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


/*
 本函数用于根据某一方的棋型，对局面作出评分
 */
int getScore(int * patterns) {
    int i;
    int score = 0;
    
    for (i = 0; i < 9; i++)
        score += patterns[i] * patternScore[i];
    
    return score;
}

/*
 本函数用于对局面作出评分，针对某一方，得到的分数为己方棋型分数减去对方棋型分数
 */
int evaluate(int color) {
    
    //这一部分用于对一些没有出现五连但胜负已分的局面针对评分者提前给出必胜或必败的评分
    //color ^ 3代表对方，color本身代表己方，针对某一方评分时，下一步棋一定轮到对方走
    if (patCurrent.pat[(color ^ 3) - 1][RushFour] || patCurrent.pat[(color ^ 3) - 1][LiveFour])   //对方有活四或者冲四，则己方必败
        return -10000000;
    else if (patCurrent.pat[color - 1][RushFour] + patCurrent.pat[color - 1][LiveFour] >= 2)      //对方没有四的情况下，己方有大于等于两个的活四或者冲四，则己方必胜
        return 10000000;
    else if (!patCurrent.pat[color - 1][RushFour] && !patCurrent.pat[color - 1][LiveFour] && patCurrent.pat[(color ^ 3) - 1][LiveThree])  //对方和己方都没有四的情况下，对方有活三，则己方必败
        return -10000000;
    
    //如果没有出现上述的情况，则正常评分
    int scoreBlack = getScore(patCurrent.pat[0]);
    int scoreWhite = getScore(patCurrent.pat[1]);
    return color == Black ? scoreBlack - scoreWhite : scoreWhite - scoreBlack;
}
