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
#define HASH      //启用置换表，若需关闭置换表请注释本行
#define HISTORY   //启用历史表，若需关闭历史表请注释本行
#define Renju     //禁手模式，若需采用无禁手模式请注释本行

#define radius 3          //落点生成半径
#define Depth 8           //搜索深度
#define Alpha -99999999      //alpha初值
#define Beta 99999999        //beta初值
#define ChildNodes 20     //最大子节点数
#define IterationDepth 2  //迭代加深起始深度
#define hashSize 16777216   //置换表大小，应取2的幂次，内存占用为14 * hashSize个字节
#define hashIndex 16777215  //应为hashSize - 1，让hashKey & hashIndex即为当前局面应在置换表中的位置

#endif /* settings_h */
