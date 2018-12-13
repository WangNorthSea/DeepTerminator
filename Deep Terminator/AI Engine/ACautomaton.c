//
//  ACautomaton.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    struct node * next[3];
    struct node * fail;
    int id;
};

//构建字典树
void insert(char * str, struct node * root, int id){
    struct node * p = root;
    int i = 0, index;
    while (str[i] != '\0') {
        index = str[i] - '0';
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
void buildFailPtr(struct node * root){
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

int * recognize(char * pattern, struct node * root, int keyCount) {
    struct node * p = root;
    int i;
    int * result = (int *)malloc(sizeof(int) * keyCount);
    result = memset(result, 0, sizeof(int) * keyCount);
    
    unsigned long int len = strlen(pattern);
    
    for (i = 0; i < len; i++) {
        int x = pattern[i] - '0';
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

void test(void) {
    char key[10];
    int i;
    struct node * root = (struct node *)malloc(sizeof(struct node));
    root -> next[0] = NULL;
    root -> next[1] = NULL;
    root -> next[2] = NULL;
    root -> fail = NULL;
    root -> id = -1;
    for (i = 0; i < 3; i++) {
        scanf("%s", key);
        insert(key, root, i);
    }
    buildFailPtr(root);
    printf("Trie and fail pointer ready.\n");
    char pattern[20];
    scanf("%s", pattern);
    int * result = recognize(pattern, root, 3);
    printf("count = %d %d %d\n", result[0], result[1], result[2]);
}
