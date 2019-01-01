//
//  main.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <string.h>
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

#ifdef Debug
int evaNodes = 0;
int hashHit = 0;
int killerHit = 0;
#endif

#ifdef Ponder
int ponderIndex = 0;
#endif

void printPats(void) {
    printf("Black:\n");
    printf("SleepOne = %d\n", patCurrent.pat[0][SleepOne]);
    printf("LiveOne = %d\n", patCurrent.pat[0][LiveOne]);
    printf("SleepTwo = %d\n", patCurrent.pat[0][SleepTwo]);
    printf("LiveTwo = %d\n", patCurrent.pat[0][LiveTwo]);
    printf("SleepThree = %d\n", patCurrent.pat[0][SleepThree]);
    printf("LiveThree = %d\n", patCurrent.pat[0][LiveThree]);
    printf("RushFour = %d\n", patCurrent.pat[0][RushFour]);
    printf("LiveFour = %d\n", patCurrent.pat[0][LiveFour]);
    printf("Five = %d\n", patCurrent.pat[0][Five]);
    
    printf("\n");
    
    printf("White:\n");
    printf("SleepOne = %d\n", patCurrent.pat[1][SleepOne]);
    printf("LiveOne = %d\n", patCurrent.pat[1][LiveOne]);
    printf("SleepTwo = %d\n", patCurrent.pat[1][SleepTwo]);
    printf("LiveTwo = %d\n", patCurrent.pat[1][LiveTwo]);
    printf("SleepThree = %d\n", patCurrent.pat[1][SleepThree]);
    printf("LiveThree = %d\n", patCurrent.pat[1][LiveThree]);
    printf("RushFour = %d\n", patCurrent.pat[1][RushFour]);
    printf("LiveFour = %d\n", patCurrent.pat[1][LiveFour]);
    printf("Five = %d\n", patCurrent.pat[1][Five]);
}

int main(void) {
    int whoWin = 0;
    int decidedIndex;
    char input[10];
    
    printf("Initializing...Please wait...\n");
    init();
    printf("Initialization finished!\n");
    
    while (1) {
        scanf("%s", input);
    enemyHasInput:
        if (!strcmp(input, "calc")) {
            if (intCount(pos) % 2 != 0)
                decidedIndex = search(board, White);
            else
                decidedIndex = search(board, Black);
            put(decidedIndex);
            
            for (int s = 0; s < intCount(pos); s++)
                printf("%s  ", transIndexToCoordinate(pos[s]));
            printf("\n");
            
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
                printf("Black wins!\n");
                removeAllPieces();
            }
            else if (whoWin == White) {
                printf("White wins!\n");
                removeAllPieces();
            }
            
#ifdef Ponder
            int lastEnemyIndex = 0;
        continuePonder:
            if (!whoWin && enemyIndex != lastEnemyIndex) {
                lastEnemyIndex = enemyIndex;
                putPiece(board, lastEnemyIndex, intCount(pos) % 2 != 0 ? White : Black);
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
                    takePiece(board, lastEnemyIndex, intCount(pos) % 2 != 0 ? White : Black);   //由于之前调用putPiece函数没有改变pos，所以实际落子数为intCount(pos) + 1
                    goto enemyHasInput;
                }
                else {  //对手落点预测成功，继续思考
                    terminateTime = time(NULL) + timeLimit;
                    pondering = 0;
                }
                pthread_join(ponderThread, NULL);
                takePiece(board, lastEnemyIndex, intCount(pos) % 2 != 0 ? White : Black);   //由于之前调用putPiece函数没有改变pos，所以实际落子数为intCount(pos) + 1
                
                put(lastEnemyIndex);
                
                whoWin = checkWhoWin();
                if (whoWin == Black) {
                    printf("Black wins!\n");
                    removeAllPieces();
                    continue;
                }
                else if (whoWin == White) {
                    printf("White wins!\n");
                    removeAllPieces();
                    continue;
                }
                
                put(ponderIndex);
                
                whoWin = checkWhoWin();
                if (whoWin == Black) {
                    printf("Black wins!\n");
                    removeAllPieces();
                    continue;
                }
                else if (whoWin == White) {
                    printf("White wins!\n");
                    removeAllPieces();
                    continue;
                }
                
                for (int s = 0; s < intCount(pos); s++)
                    printf("%s  ", transIndexToCoordinate(pos[s]));
                printf("\n");
                
                goto continuePonder;
            }
#endif
        }
        else if (!strcmp(input, "exit"))
            break;
        else if (!strcmp(input, "remove")) {
            removePiece();
#ifdef Debug
            printPats();
#endif
        }
        else {
            put(transCoordinateToIndex(input));
#ifdef Debug
            printPats();
#endif
            
            for (int s = 0; s < intCount(pos); s++)
                printf("%s  ", transIndexToCoordinate(pos[s]));
            printf("\n");
            
#ifdef Renju
            if (checkForbidMove(board, transCoordinateToIndex(input))) {
                printf("White wins! Black has made a forbidden move!\n");
                removeAllPieces();
            }
#endif
            whoWin = checkWhoWin();
            if (whoWin == Black) {
                printf("Black wins!\n");
                removeAllPieces();
            }
            else if (whoWin == White) {
                printf("White wins!\n");
                removeAllPieces();
            }
        }
    }
    
    return 0;
}
