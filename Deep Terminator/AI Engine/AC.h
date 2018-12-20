//
//  AC.h
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/20.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#ifndef AC_h
#define AC_h

struct node {
    struct node * next[3];
    struct node * fail;
    int id;
};

extern void insert(char * str, struct node * root, int id);
extern void buildFailPtr(struct node * root);
extern int * recognize(char * pattern, struct node * root, int keycharCount);

#endif /* AC_h */
