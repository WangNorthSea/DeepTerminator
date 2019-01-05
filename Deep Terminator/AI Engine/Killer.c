//
//  Killer.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/29.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include "settings.h"

struct killerNode {               //杀手走法表的单个节点，存储哈希键值与发生beta截断的着法
    unsigned long long int key;
    int index;
};

struct killerNode killer[hashSize];    //杀手走法表
