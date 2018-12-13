//
//  ArrayOp.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>

int count(char * array) {
    int i = 0;
    while (array[i] != -1) { i++; }
    return i;
}

char * append(char * array, int value) {
    int len = count(array) + 1;
    array = (char *)realloc(array, sizeof(int) * (len + 1));
    array[len - 1] = value;
    array[len] = -1;
    return array;
}

char * concat(char * first, char * second) {
    int i;
    if (!count(first)) {
        char * new = (char *)malloc(sizeof(int) * (count(second) + 1));
        for (i = 0; i < count(second); i++) {
            new[i] = second[i];
        }
        new[i] = -1;
        return new;
    }
    if (!count(second)) {
        char * new = (char *)malloc(sizeof(int) * (count(first) + 1));
        for (i = 0; i < count(first); i++) {
            new[i] = first[i];
        }
        new[i] = -1;
        return new;
    }
    int start = count(first);
    int limit = count(second) + start;
    char * new = (char *)malloc(sizeof(int) * (limit + 1));
    for (i = 0; i < start; i++) {
        new[i] = first[i];
    }
    for (i = start; i < limit; i++) {
        new[i] = second[i - start];
    }
    new[limit] = -1;
    return new;
}
