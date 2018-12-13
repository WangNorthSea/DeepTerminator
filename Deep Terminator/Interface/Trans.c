//
//  Trans.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>

int transCoordinateToIndex(char * position) {
    /* entering the "if" branch below means that the coordinate of the piece is x1 or x10 ~ x15 */
    if (position[1] == '1') {
        /* entering the "if" branch below means that the coordinate of the piece is x1 */
        if (position[2] == '\0') {
            return ((position[0] - 97) + (14 * 15));
        }
        /* entering the "else" branch below means that the coordinate of the piece is x10 ~ x15 */
        else {
            return ((15 - (10 * (position[1] - 48) + (position[2] - 48))) * 15 + (position[0] - 97));
        }
    }
    /* entering the "if" branch below means that the coordinate of the piece is x2 ~ x9 */
    else {
        return ((15 - (position[1] - 48)) * 15 + (position[0] - 97));
    }
}

char * transIndexToCoordinate(int index) {
    char * coordinate = (char *)malloc(sizeof(char) * 3);
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
