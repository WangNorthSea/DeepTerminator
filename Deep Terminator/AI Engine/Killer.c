//
//  Killer.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/29.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include "settings.h"

struct killerNode {
    unsigned long long int key;
    int index;
};

struct killerNode killer[hashSize];
