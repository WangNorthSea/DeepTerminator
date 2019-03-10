//
//  search.h
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/20.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#ifndef search_h
#define search_h

extern int cut;
extern unsigned char terminate;
extern long terminateTime;

extern int search(char * board, int color);
extern void ponder(void);

#ifdef Ponder
extern int enemyIndex;
extern unsigned char pondering;
#endif

#ifdef HISTORY
extern int historyTable[225];
#endif

#endif /* search_h */
