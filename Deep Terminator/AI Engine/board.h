//
//  board.h
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/13.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#ifndef board_h
#define board_h

#define Empty 0
#define Black 1
#define White 2
#define Extra 3
#define Stop -1

extern char board[225];
extern int * pos;

extern unsigned int * getPatternCodes(int index);

#endif /* board_h */
