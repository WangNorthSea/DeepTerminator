//
//  killer.h
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/29.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#ifndef killer_h
#define killer_h

struct killerNode {
    unsigned long long int key;
    int index;
};

extern struct killerNode killer[hashSize];

#endif /* killer_h */
