//
//  Initialize.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "settings.h"
#include "patterns.h"
#include "zobrist.h"
#include "killer.h"

unsigned char patMap[1 << 22];    //存储任何一种可能的22位二进制编码所对应的黑白双方棋型，高四位代表白子棋型，低四位代表黑子棋型

/*
 本函数用于初始化patMap数组
 */
void initPatMap(void) {
    int i;
    unsigned char line[11] = {0};  //表示一种可能的22位二进制编码，每个元素代表2位，从0号元素到10号元素分别对应22位的高位到低位，00代表空位，01代表黑子，10代表白子，11代表额外的一种情况
    for (i = 0; i < (1 << 22); i++) {   //用i遍历任何一个22位二进制数
        int temp = i;
        int j;
        
        for (j = 10; j >= 0; j--) {   //针对每一个i，将该22位二进制数划分成11个点存储进line数组
            line[j] = temp % 4;
            temp /= 4;
        }
        
        int len, space;
        unsigned short int key = 1;
        unsigned char pattern = 0;
        unsigned char * left = line + 5, * right = line + 5;
        
        //识别黑子分布
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
        pattern += patternMap[key];       //从patternMap数组取出该种黑子分布对应的黑子棋型存储到pattern的低四位
        
        key = 1;
        left = line + 5;
        right = line + 5;
        //识别白子分布
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
        pattern += patternMap[key] << 4;   //从patternMap数组取出该种白子分布对应的白子棋型存储到pattern的高四位
        patMap[i] = pattern;               //将该22位二进制数对应的黑白双方棋型存储到patMap数组，高四位代表白子，低四位代表黑子
    }
}

/*
 本函数用于初始化置换表，从hashVal数组中取出32位随机数，两两拼成一个64位随机数存入zobristMap数组中
 同时将zobristTable数组进行初始化
 */
#ifdef HASH
void initHash(void) {
    int i;
    unsigned long long int a, b;
    
    for (i = 0; i < 225; i++) {
        a = hashVal[i];
        b = hashVal[i + 225];
        zobristMap[i][0] = (a << 32) | b;
    }
    
    for (i = 0; i < 225; i++) {
        a = hashVal[i + 450];
        b = hashVal[i + 675];
        zobristMap[i][1] = (a << 32) | b;
    }
    
    for (i = 0; i < hashSize; i++) {
        zobristTable[i].key = 0;
        zobristTable[i].kind = unknown;
        zobristTable[i].depth = 0;
        zobristTable[i].score = 0;
    }
}
#endif

/*
 本函数用于初始化杀手走法表
 */
#ifdef KILLER
void initKiller(void) {
    int i;
    for (i = 0; i < hashSize; i++) {
        killer[i].key = 0;
        killer[i].index = 0;
    }
}
#endif

/*
 本函数用于在程序启动时初始化所有需要进行初始化的项目
 */
void init(void) {
    int i, j, k;
    for (i = 0; i < 225; i++)    //初始化board数组
        board[i] = Empty;
    pos = (int *)malloc(sizeof(int));  //初始化pos数组
    pos[0] = -1;
    for (i = 0; i < 2; i++) {     //初始化当前双方棋型
        for (j = 0; j < 10; j++)
            patCurrent.pat[i][j] = 0;
    }
    struct pattern initialPat;
    initialPat.pat[0][0] = 0;    //这句只是为了消除initialPat未初始化的警告
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 10; j++)
            initialPat.pat[i][j] = 0;
    }
    patHistory[0] = initialPat;   //初始化patHistory数组
    
    for (i = 0; i < 225; i++)     //初始化refreshed数组
        refreshed[i] = 0;
    
    for (i = 0; i < 225; i++) {   //初始化candidates数组
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 10; k++)
                candidates[i].pat[j][k] = 0;
        }
    }
    
    initPatMap();   //初始化patMap
#ifdef HASH
    initHash();     //初始化置换表
#endif
    
#ifdef KILLER
    initKiller();   //初始化杀手走法表
#endif
}
