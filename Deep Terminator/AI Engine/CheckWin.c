//
//  CheckWin.c
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/15.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "board.h"
#include "init.h"
#include "evaluate.h"

int checkWhoWin(void) {
    if (patCurrent.pat[0][8])
        return Black;
    if (patCurrent.pat[1][8])
        return White;
    return 0;
}
