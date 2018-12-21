//
//  init.h
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/20.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#ifndef init_h
#define init_h

extern struct node * rootBlack;
extern struct node * rootWhite;
extern struct node * rootBlackWin;
extern struct node * rootWhiteWin;
extern struct node * rootBlackRenjuWin;
extern struct node * rootForbidMove;
extern unsigned char patMap[1 << 22];

extern void init(void);

#endif /* init_h */
