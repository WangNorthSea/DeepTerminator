//
//  settings.h
//  Deep Terminator
//
//  Created by 王浩宇 on 2018/12/14.
//  Copyright © 2018 UCAS Developers. All rights reserved.
//

#ifndef settings_h
#define settings_h

#define Debug     //debug模式，若需关闭debug模式请注释本行

#define Renju     //禁手模式，若需采用无禁手模式请注释本行
#define radius 3          //落点生成半径
#define Depth 6           //搜索深度
#define Alpha -99999999      //alpha初值
#define Beta 99999999        //beta初值
#define ChildNodes 20     //最大子节点数
#define IterationDepth 2  //迭代加深起始深度
//#define EnableHistoryTable 1  //是否启用历史表

#endif /* settings_h */
