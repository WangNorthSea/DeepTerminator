//
//  ArrayOp.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>

/*
 本函数用于计算char型数组元素个数
 */
int charCount(char * array) {
    int i = 0;
    while (array[i] != -1) { i++; }
    return i;
}

/*
 本函数用于计算int型数组元素个数，只有用-1表示结尾的int型数组才可以使用
 */
int intCount(int * array) {
    int i = 0;
    while (array[i] != -1) { i++; }
    return i;
}

/*
 本函数用于为int型数组在末尾添加新元素，只有用-1表示结尾的int型数组才可以使用
 */
int * append(int * array, int value) {
    int len = intCount(array) + 1;
    array = (int *)realloc(array, sizeof(int) * (len + 1));
    array[len - 1] = value;
    array[len] = -1;
    return array;
}
