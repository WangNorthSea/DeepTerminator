//
//  Trans.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>

/*
 本函数用于将一个字符串坐标转换成棋盘数组的对应下标
 */
int transCoordinateToIndex(char * position) {
    //进入下方的if分支意味着传入的坐标为x1或者x10 ~ x15(x为a ~ o之间任意字母)
    if (position[1] == '1') {
        //进入下方的if分支意味着传入的坐标为x1
        if (position[2] == '\0') {
            return ((position[0] - 97) + (14 * 15));
        }
        //进入下方的分支意味着传入的坐标为x10 ~ x15
        else {
            return ((15 - (10 * (position[1] - 48) + (position[2] - 48))) * 15 + (position[0] - 97));
        }
    }
    //进入下方的分支意味着传入的坐标为x2 ~ x9
    else {
        return ((15 - (position[1] - 48)) * 15 + (position[0] - 97));
    }
}

/*
 本函数用于将棋盘数组的下标转换成对应的字符串坐标
 */
char * transIndexToCoordinate(int index) {
    char * coordinate = (char *)malloc(sizeof(char) * 4);
    coordinate[3] = '\0';
    coordinate[0] = index % 15 + 'a';
    if (15 - index / 15 >= 10) {
        coordinate[1] = '1';
        coordinate[2] = 5 - index / 15 + '0';
    }
    else {
        coordinate[1] = 15 - index / 15 + '0';
        coordinate[2] = '\0';
    }
    return coordinate;
}
