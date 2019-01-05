//
//  Search.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/14.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "settings.h"
#include "board.h"
#include "array.h"
#include "init.h"
#include "generate.h"
#include "win.h"
#include "renju.h"
#include "evaluate.h"
#include "zobrist.h"
#include "killer.h"
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/IO interface/trans.h"
#include "/Users/haoyuwang/Desktop/Deep Terminator/Deep Terminator/main.h"

#ifdef Debug
int cut = 0;    //记录在最深一次搜索过程中共发生了多少次beta截断
#endif

struct bestLine {    //记录每次搜索得到的双方最佳落子顺序
    int moves;
    int indexes[Depth];
};

time_t terminateTime;    //搜索应该终止的时间点
unsigned char terminate = 0;    //表示搜索是否应该终止，为1则终止搜索

#ifdef Ponder
int enemyIndex = 0;     //根据每次搜索得到的bestLine得出的对方最佳落点，用于后台思考
unsigned char pondering = 0;    //表示当前的搜索是否属于后台思考
#endif

#ifdef HISTORY
int historyTable[225];  //历史表，在搜索过程中对发生beta截断的落点以及是最佳走法的落点进行加分，用于落点排序
#endif

/*
 本函数实现基于负极大值写法的AlphaBeta剪枝搜索
 参数board为棋盘，depth为搜索深度，递归过程中会改变，color代表为哪一方搜索着法，bL用于产生bestLine，maxDepth代表本次搜索最大深度，在递归过程中不变，firstMove是基于迭代加深传入的上次浅层搜索产生的最佳着法，本次搜索先搜索此着法
 */
int alphaBeta(char * board, int depth, int alpha, int beta, int color, struct bestLine * bL, int maxDepth, int firstMove) {
#ifdef HASH
    int indexInHash = hashKey & hashIndex;      //首先检查置换表是否命中
    if (depth < maxDepth) {      //只有在非根节点才进行检查
        if (zobristTable[indexInHash].key == hashKey) {
            if (zobristTable[indexInHash].depth == depth) { //depth == 0时置换表不写入，所以depth == 0时不会命中overNode以外的的局面
#ifdef Debug
                hashHit++;
#endif
                if (zobristTable[indexInHash].kind == betaNode) {
                    if (zobristTable[indexInHash].score >= beta)    //如果存储的节点在当前仍然是beta节点
                        return beta;
                }
                else if (zobristTable[indexInHash].kind == alphaNode) {
                    if (zobristTable[indexInHash].score <= alpha)   //如果存储的节点在当前仍然是alpha节点
                        return alpha;
                }
                else if (zobristTable[indexInHash].kind == PVNode) {
                    if (zobristTable[indexInHash].score < beta && zobristTable[indexInHash].score > alpha)   //如果存储的节点在当前仍然是PV节点
                        return zobristTable[indexInHash].score;
                }
            }
            else if (zobristTable[indexInHash].kind == overNode && depth < maxDepth) {   //如果是必胜或必败节点则不看深度
#ifdef Debug
                hashHit++;
#endif
                if (zobristTable[indexInHash].score == 10000000) {   //是必胜节点
                    if  (beta == Beta)    //beta还等于初值
                        return depth == 0 ? -10000000 : 10000000;  //最深一层节点比较特殊，此时进行评估针对的一方与必胜必败节点针对的一方是正好相反的
                    else
                        return depth == 0 ? -10000000 : beta;      //如果beta已经不等于初值，那么beta必定小于等于10000000，如果必胜，返回beta值即可
                }
                else {    //是必败节点
                    if (alpha == Alpha)   //alpha还等于初值
                        return depth == 0 ? 10000000 : -10000000;  //最深一层节点比较特殊，此时进行评估针对的一方与必胜必败节点针对的一方是正好相反的
                    else
                        return depth == 0 ? 10000000 : alpha;      //如果alpha已经不等于初值，那么必败时应该直接返回alpha值
                }
            }
        }
    }
#endif
    if (depth == 0) {
#ifdef Debug
        evaNodes++;
#endif
        
        if (terminateTime - time(NULL) <= 1)   //剩余时间不到1秒，则立即终止搜索
            terminate = 1;
        
        bL -> moves = 0;
        if (color == Black)   //depth == 0时轮到哪一方走棋，就应该对另一方进行评价
            return evaluate(White);
        else
            return evaluate(Black);
    }
    
    int i;
    int score;
    int whoWin = 0;
    int tempIndex = 0;
    int foundPV = 0;                 //是否找到主要变例(PV)
    unsigned char foundForbid = 0;   //是否发现禁手点
#ifdef HASH
    int maxValue = -99999999;
#endif
#ifdef HISTORY
    int bestMove = 0;
#endif
    struct bestLine bestL;
    bestL.moves = 0;
    
    for (i = 0; i < Depth; i++)
        bestL.indexes[i] = 0;
    
    int * indexArray;
    if (depth < maxDepth)      //根节点时，按照半径为3选择空位，其余节点按照半径为2选择空位
        indexArray = generateCAND(board, color, 1);
    else
        indexArray = generateCAND(board, color, 0);
    int indexCount = intCount(indexArray);
    
    if (maxDepth > IterationDepth && depth == maxDepth) {   //如果不是第一次迭代且在根节点，则通过冒泡的方式让firstMove变成第一个搜索的着法
        for (i = 0; i < indexCount; i++) {
            if (indexArray[i] == firstMove) {
                int j = i;
                while (j > 0) {
                    tempIndex = indexArray[j];
                    indexArray[j] = indexArray[j - 1];
                    indexArray[j - 1] = tempIndex;
                    j--;
                }
                break;
            }
        }
    }
    
#ifdef KILLER
    if (depth < maxDepth) {  //在非根节点
        if (killer[indexInHash].key == hashKey) {    //如果当前节点命中杀手走法表
            for (i = 0; i < indexCount; i++) {
                if (indexArray[i] == killer[indexInHash].index) {    //让杀手走法通过冒泡的方式变成第一个搜索的着法
#ifdef Debug
                    killerHit++;
#endif
                    int j = i;
                    while (j > 0) {
                        tempIndex = indexArray[j];
                        indexArray[j] = indexArray[j - 1];
                        indexArray[j - 1] = tempIndex;
                        j--;
                    }
                    break;
                }
            }
        }
    }
#endif
    
    if (depth == maxDepth) {    //在根节点处先选择一个点作为bestLine第一个点，防止出现全部命中置换表导致一个点也没有的情况
        bL -> indexes[0] = indexArray[0];
        memcpy(bL -> indexes + 1, bestL.indexes, sizeof(int) * (bestL.moves));
        bL -> moves = bestL.moves + 1;
    }
    
    for (i = 0; i < indexCount; i++) {
        
        if (terminate)   //如果需要终止搜索，则直接退出循环
            break;
        
        putPiece(board, indexArray[i], color);   //按着法顺序落子
        
#ifdef Renju
        if (color == Black) {    //在黑子下棋的节点，检查每一个着法是否触犯禁手，如果触犯禁手，则直接跳过
            if (checkForbidMove(board, indexArray[i])) {
                takePiece(board, indexArray[i], color);
                foundForbid = 1;  //发现禁手着法
                continue;
            }
        }
#endif
        
        whoWin = checkWhoWin();   //检查落子后有没有出现胜利局面

        if (whoWin) {
            if (whoWin == color)
                score = 10000000;
            else
                score = -10000000;
            goto someoneWin;     //出现胜利局面则不再从此节点向下搜索
        }
        
        if (foundPV) {   //发现主要变例则进行主要变例搜索
            if (color == Black)
                score = -alphaBeta(board, depth - 1, -alpha - 1, -alpha, White, &bestL, maxDepth, firstMove);  //进行PVS
            else
                score = -alphaBeta(board, depth - 1, -alpha - 1, -alpha, Black, &bestL, maxDepth, firstMove);
            if (depth == 1)
                score = -score;
            
            if (score > alpha && score < beta) {  //PVS失败，则重新进行正常搜索
                if (color == Black)
                    score = -alphaBeta(board, depth - 1, -beta, -alpha, White, &bestL, maxDepth, firstMove);
                else
                    score = -alphaBeta(board, depth - 1, -beta, -alpha, Black, &bestL, maxDepth, firstMove);
                if (depth == 1)
                    score = -score;
            }
        }
        else {   //未发现主要变例则进行正常搜索
            if (color == Black)
                score = -alphaBeta(board, depth - 1, -beta, -alpha, White, &bestL, maxDepth, firstMove);
            else
                score = -alphaBeta(board, depth - 1, -beta, -alpha, Black, &bestL, maxDepth, firstMove);
            if (depth == 1)
                score = -score;
        }
        
    someoneWin:
        takePiece(board, indexArray[i], color);   //移除落下的棋子
        
        if (score == Alpha || score == Beta)      //消除由于中途终止搜索而可能产生的分数bug
            score = alpha;
        
#ifdef Debug
#ifdef Ponder
        if (depth == maxDepth && !pondering)
            printf("%s = %d\n", transIndexToCoordinate(indexArray[i]), score);
#else
        if (depth == maxDepth)
            printf("%s = %d\n", transIndexToCoordinate(indexArray[i]), score);
#endif
#endif
        
        if (score >= beta) {   //发生beta截断
#ifdef Debug
            cut++;
#endif
#ifdef HASH
            zobristTable[indexInHash].key = hashKey;
            if (score == 10000000) {                           //如果是因为必胜而发生beta截断，则将当前节点在置换表中保存为必胜必败节点，否则保存为beta节点
                zobristTable[indexInHash].kind = overNode;
                zobristTable[indexInHash].depth = 100;         //必胜必败节点不看深度
            }
            else {
                zobristTable[indexInHash].kind = betaNode;
                zobristTable[indexInHash].depth = depth;
            }
            zobristTable[indexInHash].score = score;
#endif
            
#ifdef KILLER
            killer[indexInHash].key = hashKey;              //将发生beta截断的落点在杀手走法表中保存为当前节点的杀手走法
            killer[indexInHash].index = indexArray[i];
#endif
            
#ifdef HISTORY
            historyTable[indexArray[i]] += depth * depth;   //将发生beta截断的落点在历史表中加分
#endif
            free(indexArray);
            return beta;
        }
        if (score > alpha) {     //落点为主要变例
            alpha = score;
            foundPV = 1;         //发现主要变例
#ifdef HISTORY
            bestMove = indexArray[i];
#endif
            bL -> indexes[0] = indexArray[i];
            memcpy(bL -> indexes + 1, bestL.indexes, sizeof(int) * (bestL.moves));
            bL -> moves = bestL.moves + 1;
        }
#ifdef HASH
        if (maxValue < score)
            maxValue = score;   //maxValue始终为搜索得到的最高分
#endif
    }
    
    if (indexArray[1] == -1 && foundForbid) {    //黑子只有一种着法然而触犯禁手，即被抓了禁手，此时黑子必败
        free(indexArray);
        return -10000000;
    }
    
#ifdef HASH
    if (maxValue == -10000000) {    //因为必胜局面必然触发beta截断，所以到这里的必胜必败局面只能是必败局面
        zobristTable[indexInHash].key = hashKey;
        zobristTable[indexInHash].kind = overNode;
        zobristTable[indexInHash].depth = 100;
        zobristTable[indexInHash].score = maxValue;
    }
    else if (foundPV) {    //不是必败局面，如果发现了主要变例，则为PV节点
        zobristTable[indexInHash].key = hashKey;
        zobristTable[indexInHash].kind = PVNode;
        zobristTable[indexInHash].depth = depth;
        zobristTable[indexInHash].score = maxValue;
    }
    else {    //都不是则为alpha节点
        zobristTable[indexInHash].key = hashKey;
        zobristTable[indexInHash].kind = alphaNode;
        zobristTable[indexInHash].depth = depth;
        zobristTable[indexInHash].score = maxValue;
        
#ifdef KILLER
        if (killer[indexInHash].key == hashKey) {   //如果当前节点为alpha节点且命中了杀手走法表，说明当前节点已不再是beta节点，应从杀手走法表中移除
            killer[indexInHash].key = 0;
            killer[indexInHash].index = 0;
        }
#endif
    }
#endif
    
#ifdef HISTORY
    if (bestMove)
        historyTable[bestMove] += depth * depth;    //对于非beta节点，则对最好的着法在历史表中加分
#endif
    free(indexArray);
    return alpha;
}

/*
 本函数用于初始化历史表
 */
#ifdef HISTORY
void initHistoryTable(void) {
    int i;
    for (i = 0; i < 225; i++)
        historyTable[i] = 0;
}
#endif

/*
 本函数用于针对输入的棋盘与棋子颜色，搜索出一个最佳落点并产生bestLine
 */
int search(char * board, int color) {
    int i, j;
    int bestScore = 0;
    int decidedIndex = 0;         //搜索得到的最佳着法
    char boardToSearch[225];      //因为alphaBeta函数内部需要更改棋盘数组，所以传一个复制出来的棋盘进去供其修改
    int bests[Depth] = {0};       //bestLine走法
    struct bestLine * bL = NULL;
    
#ifdef Ponder
    terminateTime = pondering ? time(NULL) + 999 : time(NULL) + timeLimit;    //如果当前处于后台思考状态，则不限时间，否则将搜索时间限制在timeLimit秒
#else
    terminateTime = time(NULL) + timeLimit;
#endif
    terminate = 0;      //开始搜索时，terminate为0
    
    for (i = 0; i < 225; i++)
        boardToSearch[i] = board[i];   //复制棋盘
    
#ifdef HISTORY
    initHistoryTable();    //每步棋搜索前初始化历史表
#endif
    for (i = IterationDepth; i <= Depth; i++) {
        for (j = 0; j < 225; j++)  //每次迭代加深前标记所有的点为需要更新棋型状态
            refreshed[j] = 0;
        
        bL = (struct bestLine *)malloc(sizeof(struct bestLine));
        bL -> moves = 0;
        for (j = 0; j < Depth; j++)
            bL -> indexes[j] = 0;
        
        bestScore = alphaBeta(boardToSearch, i, Alpha, Beta, color, bL, i, bests[0]);   //进行搜索
        
        for (j = 0; j < Depth; j++) {
            if (j == 1 && terminate)    //如果当前bestLine是通过中途终止搜索得到的，则不改变bests[1]，以防止后台思考出现bug
                continue;
            bests[j] = bL -> indexes[j];
        }
        
        decidedIndex = bests[0];    //bests[0]即为搜索得到的最佳着法
        
#ifdef Debug
        if (color == Black)
            printf("Black:%s  Score:%d\n", transIndexToCoordinate(decidedIndex), bestScore);
        else
            printf("White:%s  Score:%d\n", transIndexToCoordinate(decidedIndex), bestScore);
        
        printf("bestline:");
        for (j = 0; j < Depth; j++) {
            if (!bests[j])
                break;
            printf("%s ", transIndexToCoordinate(bests[j]));
        }
        printf("\n");
#endif
        if (bestScore >= 10000000 || bestScore <= -10000000)  //已经出现必胜或必败，则不再迭代
            break;
        
        if (terminate)     //如果搜索已经终止，则不再迭代
            break;
        
        if (i == 8 && terminateTime - time(NULL) <= 5)   //如果8层搜索结束后剩余时间已经不到5秒，则不再进行9层、10层的搜索
            break;
            
        if (i < Depth) {
#ifdef Debug
            hashHit = 0;
            evaNodes = 0;
            cut = 0;
            killerHit = 0;
#endif
            free(bL);
        }
    }
    
#ifdef Ponder
    if (bests[1] && bests[0] != bests[1])    //如果bests[1]不为0且不与bests[0]相同，则将其选为后台思考时对方落子点
        enemyIndex = bests[1];               //这是因为己方下bests[0]点时对方的落子点可能命中置换表从而没有修改bestLine中对应位置
#endif
    
    free(bL);
    return decidedIndex;
}

/*
 本函数用于启动后台思考，进行后台思考的线程调用本函数
 */
#ifdef Ponder
void ponder(void) {
    int decidedIndex;
    pondering = 1;       //标记当前搜索状态为后台思考
    decidedIndex = search(board, intCount(pos) % 2 != 0 ? Black : White);   //由于之前调用putPiece函数没有改变pos，所以实际落子数为intCount(pos) + 1
    if (!pondering)      //此时pondering == 0说明搜索被中途终止
        ponderIndex = decidedIndex;
    else {
        pondering = 0;
        ponderIndex = decidedIndex;
    }
}
#endif
