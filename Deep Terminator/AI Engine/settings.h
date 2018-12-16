//
//  settings.h
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/14.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#ifndef settings_h
#define settings_h

#define Renju 0    //规则，为1有禁手，为0无禁手

#define radius 3          //落点生成半径
#define Depth 6           //搜索深度
#define Alpha -99999      //alpha初值
#define Beta 99999        //beta初值
#define ChildNodes 20     //最大子节点数
#define IterationDepth 2  //迭代加深起始深度
#define EnableHistoryTable 1  //是否启用历史表

#endif /* settings_h */
