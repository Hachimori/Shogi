/*
 * Constants.h
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include<map>

const int SIZE = 3;
const int LOOP = 1000;


enum Type{
    BLANK,   // 空欄
    HU,      // 歩
    KYOSYA,  // 香
    KEIMA,   // 桂
    GIN,     // 銀
    KIN,     // 金
    HISYA,   // 飛
    KAKU,    // 角
    GYOKU    // 玉
};

#endif /* CONSTANTS_H_ */
