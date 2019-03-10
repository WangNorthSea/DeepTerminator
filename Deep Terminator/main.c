//
//  main.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <pthread.h>
#include "AIEngine/settings.h"
#include "AIEngine/board.h"
#include "AIEngine/array.h"
#include "AIEngine/init.h"
#include "AIEngine/search.h"
#include "AIEngine/win.h"
#include "AIEngine/renju.h"
#include "IOinterface/changeBoard.h"
#include "IOinterface/trans.h"
#include "AIEngine/patterns.h"
#include "CLI/cli.h"

#ifdef Debug
int evaNodes = 0;         //记录搜索过程中评估了多少局面
int hashHit = 0;          //记录搜索过程中置换表命中次数
int killerHit = 0;        //记录搜索过程中杀手走法表命中次数
#endif

#ifdef Ponder
int ponderIndex = 0;      //用于后台思考的对方落子点
#endif

/*
 本函数用于检查当前是否已平局
 */
int checkDraw(void) {
    return intCount(pos) == 225 ? 1 : 0;
}

/*
 本函数用于进行人机对战，参数AIPiece为AI所执的棋子
 */
void manVSAI(int AIPiece) {
    int whoWin = 0;
    int decidedIndex = 0;       //搜索得到的最佳着法
    char input[10];
    
    if (AIPiece == Black) {
        put(transCoordinateToIndex("h8"));    //开局先在h8落子
        printBoard();
        
        while (1) {
            scanf("%s", input);
        enemyHasInput:
            if (!strcmp(input, "exit"))       //输入的是退出命令
                break;
            else if (!strcmp(input, "remove")) {     //输入的是悔棋命令
                removePiece();        //因为要先删掉AI计算出来的落子，再删掉玩家落下的子，所以removePiece要调用两遍
                removePiece();
                printBoard();
            }
            else {
                put(transCoordinateToIndex(input));   //输入的是落子坐标
                printBoard();
                
                whoWin = checkWhoWin();            //检查胜利
                if (whoWin == Black) {
                    printf("\nBlack wins!\n");
                    break;
                }
                else if (whoWin == White) {
                    printf("\nWhite wins!\n");
                    break;
                }
                
                if (checkDraw()) {                //检查平局
                    printf("\nDraw!\n");
                    break;
                }
                
                printf("\nAI is thinking...Please wait...\n");
                decidedIndex = search(board, Black);        //AI进行搜索
                put(decidedIndex);
                
                printBoard();
                
#ifdef Debug
                printf("evaluated nodes = %d\n", evaNodes);
                printf("alphaBeta cut = %d\n", cut);
                printf("hashHit = %d\n", hashHit);
                printf("killerHit = %d\n", killerHit);
                hashHit = 0;
                evaNodes = 0;
                cut = 0;
                killerHit = 0;
#endif
                
                whoWin = checkWhoWin();
                if (whoWin == Black) {
                    printf("\nBlack wins!\n");
                    break;
                }
                else if (whoWin == White) {
                    printf("\nWhite wins!\n");
                    break;
                }
                
                if (checkDraw()) {
                    printf("\nDraw!\n");
                    break;
                }
                
#ifdef Ponder
                int lastEnemyIndex = 0;
                int ponderCheckLoop;
            continuePonder:
                for (ponderCheckLoop = 0; ponderCheckLoop < intCount(pos); ponderCheckLoop++) {    //如果后台思考要用的对方落点已经有棋子了，则不进行后台思考，这是因为由于置换表的存在，bestLine的获取不是十分稳定，有时候会出bug
                    if (enemyIndex == pos[ponderCheckLoop])
                        break;
                }

                if (!whoWin && enemyIndex != lastEnemyIndex && ponderCheckLoop == intCount(pos)) {  //如果没有胜利且enemyIndex正常，则进行后台思考
                    lastEnemyIndex = enemyIndex;
                    putPiece(board, lastEnemyIndex, White);
                    pthread_t ponderThread;
                    pthread_create(&ponderThread, NULL, (void *)ponder, NULL);
                    
                    char enemyInput[10];
                    scanf("%s", enemyInput);
                    
                    if (transCoordinateToIndex(enemyInput) != lastEnemyIndex) {  //对手落点预测失败，终止思考
                        terminate = 1;
                        pthread_join(ponderThread, NULL);
                        int loop;
                        for (loop = 0; loop < 10; loop++)
                            input[loop] = enemyInput[loop];
                        takePiece(board, lastEnemyIndex, White);   //由于之前调用putPiece函数没有改变pos，所以实际落子数为intCount(pos) + 1
                        goto enemyHasInput;
                    }
                    else {  //对手落点预测成功，继续思考
                        printf("\nAI is thinking...Please wait...\n");
                        terminateTime = time(NULL) + timeLimit;
                        pondering = 0;
                    }
                    pthread_join(ponderThread, NULL);
                    takePiece(board, lastEnemyIndex, White);   //由于之前调用putPiece函数没有改变pos，所以实际落子数为intCount(pos) + 1
                    
                    put(lastEnemyIndex);    //落下对方的棋子
                    
                    printBoard();
                    
                    whoWin = checkWhoWin();
                    if (whoWin == Black) {
                        printf("\nBlack wins!\n");
                        break;
                    }
                    else if (whoWin == White) {
                        printf("\nWhite wins!\n");
                        break;
                    }
                    
                    if (checkDraw()) {
                        printf("\nDraw!\n");
                        break;
                    }
                    
                    put(ponderIndex);     //落下后台思考得到的着法
                    
                    printBoard();
                    
                    whoWin = checkWhoWin();
                    if (whoWin == Black) {
                        printf("\nBlack wins!\n");
                        break;
                    }
                    else if (whoWin == White) {
                        printf("\nWhite wins!\n");
                        break;
                    }
                    
                    if (checkDraw()) {
                        printf("\nDraw!\n");
                        break;
                    }
                    
                    goto continuePonder;    //预测成功后用新得到的对方最佳着法继续进行后台思考
                }
#endif
            }
        }
    }
    else {
        printBoard();
        while (1) {
            scanf("%s", input);
        enemyHasInputBlack:
            if (!strcmp(input, "exit"))
                break;
            else if (!strcmp(input, "remove")) {
                removePiece();
                removePiece();
                printBoard();
            }
            else {
                put(transCoordinateToIndex(input));
                printBoard();
                
                whoWin = checkWhoWin();
                if (whoWin == Black) {
                    printf("\nBlack wins!\n");
                    break;
                }
                else if (whoWin == White) {
                    printf("\nWhite wins!\n");
                    break;
                }
                
                if (checkDraw()) {
                    printf("\nDraw!\n");
                    break;
                }
#ifdef Renju
                if (checkForbidMove(board, transCoordinateToIndex(input))) {       //因为玩家执黑，所以要检查黑棋是否触犯禁手
                    printf("\nWhite wins! Black has made a forbidden move!\n");
                    break;
                }
#endif
                
                printf("\nAI is thinking...Please wait...\n");
                decidedIndex = search(board, White);
                put(decidedIndex);
                printBoard();
                
#ifdef Debug
                printf("evaluated nodes = %d\n", evaNodes);
                printf("alphaBeta cut = %d\n", cut);
                printf("hashHit = %d\n", hashHit);
                printf("killerHit = %d\n", killerHit);
                hashHit = 0;
                evaNodes = 0;
                cut = 0;
                killerHit = 0;
#endif
                whoWin = checkWhoWin();
                if (whoWin == Black) {
                    printf("\nBlack wins!\n");
                    break;
                }
                else if (whoWin == White) {
                    printf("\nWhite wins!\n");
                    break;
                }
                
                if (checkDraw()) {
                    printf("\nDraw!\n");
                    break;
                }
#ifdef Ponder
                int lastEnemyIndex = 0;
                int ponderCheckLoop;
            continuePonderWhite:
                for (ponderCheckLoop = 0; ponderCheckLoop < intCount(pos); ponderCheckLoop++) {
                    if (enemyIndex == pos[ponderCheckLoop])
                        break;
                }
                
                if (!whoWin && enemyIndex != lastEnemyIndex && ponderCheckLoop == intCount(pos)) {
                    lastEnemyIndex = enemyIndex;
                    putPiece(board, lastEnemyIndex, Black);
                    pthread_t ponderThread;
                    pthread_create(&ponderThread, NULL, (void *)ponder, NULL);
                    
                    char enemyInput[10];
                    scanf("%s", enemyInput);
                    
                    if (transCoordinateToIndex(enemyInput) != lastEnemyIndex) {  //对手落点预测失败，终止思考
                        terminate = 1;
                        pthread_join(ponderThread, NULL);
                        int loop;
                        for (loop = 0; loop < 10; loop++)
                            input[loop] = enemyInput[loop];
                        takePiece(board, lastEnemyIndex, Black);   //由于之前调用putPiece函数没有改变pos，所以实际落子数为intCount(pos) + 1
                        goto enemyHasInputBlack;
                    }
                    else {  //对手落点预测成功，继续思考
                        printf("\nAI is thinking...Please wait...\n");
                        terminateTime = time(NULL) + timeLimit;
                        pondering = 0;
                    }
                    pthread_join(ponderThread, NULL);
                    takePiece(board, lastEnemyIndex, Black);   //由于之前调用putPiece函数没有改变pos，所以实际落子数为intCount(pos) + 1
                    
                    put(lastEnemyIndex);
                    
                    printBoard();
                    
                    whoWin = checkWhoWin();
                    if (whoWin == Black) {
                        printf("\nBlack wins!\n");
                        break;
                    }
                    else if (whoWin == White) {
                        printf("\nWhite wins!\n");
                        break;
                    }
#ifdef Renju
                    if (checkForbidMove(board, lastEnemyIndex)) {
                        printf("\nWhite wins! Black has made a forbidden move!\n");
                        break;
                    }
#endif
                    if (checkDraw()) {
                        printf("\nDraw!\n");
                        break;
                    }
                    
                    put(ponderIndex);
                    
                    printBoard();
                    
                    whoWin = checkWhoWin();
                    if (whoWin == Black) {
                        printf("\nBlack wins!\n");
                        break;
                    }
                    else if (whoWin == White) {
                        printf("\nWhite wins!\n");
                        break;
                    }
                    
                    if (checkDraw()) {
                        printf("\nDraw!\n");
                        break;
                    }
                    
                    goto continuePonderWhite;
                }
#endif
            }
        }
    }
}

/*
 本函数用于进行人人对战
 */
void manVSMan(void) {
    int whoWin = 0;
    int decidedIndex = 0;
    char input[10];
    
    printBoard();
    while (1) {
        scanf("%s", input);
    enemyHasInput:
        if (!strcmp(input, "calc")) {      //输入的是计算指令
            printf("AI is thinking...Please wait...\n");
            if (intCount(pos) % 2 != 0)
                decidedIndex = search(board, White);
            else
                decidedIndex = search(board, Black);
            put(decidedIndex);
            printBoard();
#ifdef Debug
            printf("evaluated nodes = %d\n", evaNodes);
            printf("alphaBeta cut = %d\n", cut);
            printf("hashHit = %d\n", hashHit);
            printf("killerHit = %d\n", killerHit);
            hashHit = 0;
            evaNodes = 0;
            cut = 0;
            killerHit = 0;
#endif
            whoWin = checkWhoWin();
            if (whoWin == Black) {
                printf("\nBlack wins!\n");
                break;
            }
            else if (whoWin == White) {
                printf("\nWhite wins!\n");
                break;
            }
            
            if (checkDraw()) {
                printf("\nDraw!\n");
                break;
            }
        }
        else if (!strcmp(input, "exit"))
            break;
        else if (!strcmp(input, "remove")) {
            removePiece();
            printBoard();
        }
        else {
            put(transCoordinateToIndex(input));
            printBoard();
#ifdef Renju
            if (checkForbidMove(board, transCoordinateToIndex(input))) {
                printf("\nWhite wins! Black has made a forbidden move!\n");
                break;
            }
#endif
            whoWin = checkWhoWin();
            if (whoWin == Black) {
                printf("\nBlack wins!\n");
                break;
            }
            else if (whoWin == White) {
                printf("\nWhite wins!\n");
                break;
            }
            
            if (checkDraw()) {
                printf("\nDraw!\n");
                break;
            }
        }
    }
}

int main(void) {
    char input[10];
    
    setlocale(LC_ALL,"zh_CN.UTF-8");     //设置地域为中国，Windows下应改为chs
    
    printf("Initializing...Please wait...\n");
    init();                                         //进行初始化
    printf("Initialization finished!\n");
    
    printf("请选择游戏模式\nA.人机对战\nB.人人对战\n请输入:");
    while (1) {
        scanf("%s", input);
        if (!strcmp(input, "A")) {
            printf("请选择先后手\nA.计算机执黑\nB.计算机执白\n请输入:");
            while (1) {
                scanf("%s", input);
                if (!strcmp(input, "A")) {
                    manVSAI(Black);
                    break;
                }
                else if (!strcmp(input, "B")) {
                    manVSAI(White);
                    break;
                }
                else
                    printf("请重新输入:");
            }
            break;
        }
        else if (!strcmp(input, "B")) {
            manVSMan();
            break;
        }
        else
            printf("请重新输入:");
    }
    
    return 0;
}
