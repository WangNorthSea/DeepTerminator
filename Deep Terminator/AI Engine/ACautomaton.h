//
//  ACautomaton.h
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/14.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#ifndef ACautomaton_h
#define ACautomaton_h

struct node {
    struct node * next[3];
    struct node * fail;
    int id;
};

#endif /* ACautomaton_h */
