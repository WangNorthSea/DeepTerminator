//
//  zobrist.h
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/22.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#ifndef zobrist_h
#define zobrist_h

#define unknown 0
#define betaNode 1
#define PVNode 2
#define alphaNode 3
#define overNode 4

struct situation {
    unsigned long long int key;
    unsigned char kind;
    unsigned char depth;
    int score;
};

extern unsigned long long int hashKey;
extern struct situation zobristTable[hashSize];
extern unsigned long long int zobristMap[225][2];

#endif /* zobrist_h */
