//
//  ArrayOp.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>

int charCount(char * array) {
    int i = 0;
    while (array[i] != -1) { i++; }
    return i;
}

int intCount(int * array) {
    int i = 0;
    while (array[i] != -1) { i++; }
    return i;
}

int * append(int * array, int value) {
    int len = intCount(array) + 1;
    array = (int *)realloc(array, sizeof(int) * (len + 1));
    array[len - 1] = value;
    array[len] = -1;
    return array;
}

/*char * concat(char * first, char * second) {
    int i;
    if (!charCount(first)) {
        char * new = (char *)malloc(sizeof(int) * (charCount(second) + 1));
        for (i = 0; i < charCount(second); i++) {
            new[i] = second[i];
        }
        new[i] = -1;
        return new;
    }
    if (!charCount(second)) {
        char * new = (char *)malloc(sizeof(int) * (charCount(first) + 1));
        for (i = 0; i < charCount(first); i++) {
            new[i] = first[i];
        }
        new[i] = -1;
        return new;
    }
    int start = charCount(first);
    int limit = charCount(second) + start;
    char * new = (char *)malloc(sizeof(int) * (limit + 1));
    for (i = 0; i < start; i++) {
        new[i] = first[i];
    }
    for (i = start; i < limit; i++) {
        new[i] = second[i - start];
    }
    new[limit] = -1;
    return new;
}*/
