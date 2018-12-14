//
//  ACautomaton.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "ACautomaton.h"

extern int charCount(char * array);

//构建字典树
void insert(char * str, struct node * root, int id) {
    struct node * p = root;
    int i = 0, index;
    while (str[i] != -1) {
        index = str[i];
        if (p -> next[index] == NULL) {
            p -> next[index] = (struct node *)malloc(sizeof(struct node));
            p -> next[index] -> next[0] = NULL;
            p -> next[index] -> next[1] = NULL;
            p -> next[index] -> next[2] = NULL;
            p -> next[index] -> fail = NULL;
            p -> next[index] -> id = -1;
        }
        p = p -> next[index];
        i++;
    }
    p -> id = id;     //在单词的最后一个节点id + 1，代表一个单词
}

//构建失败指针
void buildFailPtr(struct node * root) {
    int i;
    struct node * q[1000];    //队列，用于构造失败指针
    int head = 0, tail = 1;
    root -> fail = NULL;
    q[head] = root;
    while (head != tail) {
        struct node * temp = q[head];
        head++;
        struct node * p = NULL;
        for (i = 0; i < 3; i++) {
            if (temp -> next[i] != NULL) {
                if (temp == root)
                    temp -> next[i] -> fail = root;
                else {
                    p = temp -> fail;
                    while (p != NULL) {
                        if (p -> next[i] != NULL) {
                            temp -> next[i] -> fail = p -> next[i];
                            break;
                        }
                        p = p -> fail;
                    }
                    if (p == NULL)
                        temp -> next[i] -> fail = root;
                }
                q[tail] = temp -> next[i];
                tail++;
            }
        }
    }
}

int * recognize(char * pattern, struct node * root, int keycharCount) {
    struct node * p = root;
    int i;
    int * result = (int *)malloc(sizeof(int) * keycharCount);
    result = memset(result, 0, sizeof(int) * keycharCount);
    
    int len = charCount(pattern);
    
    for (i = 0; i < len; i++) {
        int x = pattern[i];
        while (p -> next[x] == NULL && p != root)
            p = p -> fail;
        p = p -> next[x];
        if (p == NULL)
            p = root;
        struct node * temp = p;
        while (temp != root) {
            if (temp -> id >= 0)
                result[temp -> id]++;
            temp = temp -> fail;
        }
    }
    
    return result;
}
