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
#include "AI Engine/settings.h"
#include "AI Engine/board.h"
#include "AI Engine/array.h"
#include "AI Engine/init.h"
#include "AI Engine/search.h"
#include "AI Engine/win.h"
#include "AI Engine/renju.h"
#include "IO interface/changeBoard.h"
#include "IO interface/trans.h"
#include "AI Engine/patterns.h"
#include "CLI/cli.h"

#ifdef Debug
int evaNodes = 0;
int hashHit = 0;
int killerHit = 0;
#endif

#ifdef Ponder
int ponderIndex = 0;
#endif

void manVSAI(int AIPiece) {
    int whoWin = 0;
    int decidedIndex = 0;
    char input[10];
    
    if (AIPiece == Black) {
        put(transCoordinateToIndex("h8"));
        printBoard();
        
        while (1) {
            scanf("%s", input);
        enemyHasInput:
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
                
                printf("\nAI is thinking...Please wait...\n");
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
                
#ifdef Ponder
                int lastEnemyIndex = 0;
                int ponderCheckLoop;
            continuePonder:
                for (ponderCheckLoop = 0; ponderCheckLoop < intCount(pos); ponderCheckLoop++) {
                    if (enemyIndex == pos[ponderCheckLoop])
                        break;
                }

                if (!whoWin && enemyIndex != lastEnemyIndex && ponderCheckLoop == intCount(pos)) {
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
                    
                    goto continuePonder;
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
                
#ifdef Renju
                if (checkForbidMove(board, transCoordinateToIndex(input))) {
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
                    
#ifdef Renju
                    if (checkForbidMove(board, lastEnemyIndex)) {
                        printf("\nWhite wins! Black has made a forbidden move!\n");
                        break;
                    }
#endif
                    
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
                    
                    goto continuePonderWhite;
                }
#endif
            }
        }
    }
}

void manVSMan(void) {
    int whoWin = 0;
    int decidedIndex = 0;
    char input[10];
    
    printBoard();
    while (1) {
        scanf("%s", input);
    enemyHasInput:
        if (!strcmp(input, "calc")) {
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
        }
    }
}

int main(void) {
    char input[10];
    
    setlocale(LC_ALL,"zh_CN.UTF-8");
    
    printf("Initializing...Please wait...\n");
    init();
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
