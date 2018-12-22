//
//  Zobrist.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/22.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include "settings.h"

struct situation {
    unsigned long long int key;
    unsigned char kind;
    unsigned char depth;
    int score;
};

unsigned long long int hashKey = 0;
struct situation zobristTable[hashSize];
unsigned long long int zobristMap[225][2];  //第二个下标0代表黑子，1代表白子
