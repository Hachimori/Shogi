/*
 * TypeUtil.cpp
 *
 *  Created on: 2016/09/08
 *      Author: Ben
 */

#include<string>
#include"TypeUtil.h"
using namespace std;


void typeutil::init() {
    TYPENAME_TO_ID["BLANK"] = BLANK;
    TYPENAME_TO_ID["HU"] = HU;
    TYPENAME_TO_ID["KYOSYA"] = KYOSYA;
    TYPENAME_TO_ID["KEIMA"] = KEIMA;
    TYPENAME_TO_ID["GIN"] = GIN;
    TYPENAME_TO_ID["KIN"] = KIN;
    TYPENAME_TO_ID["HISYA"] = HISYA;
    TYPENAME_TO_ID["KAKU"] = KAKU;
    TYPENAME_TO_ID["GYOKU"] = GYOKU;

    ID_TO_TYPENAME[BLANK] = "BLANK";
    ID_TO_TYPENAME[HU] = "HU";
    ID_TO_TYPENAME[KYOSYA] = "KYOSYA";
    ID_TO_TYPENAME[KEIMA] = "KEIMA";
    ID_TO_TYPENAME[GIN] = "GIN";
    ID_TO_TYPENAME[KIN] = "KIN";
    ID_TO_TYPENAME[HISYA] = "HISYA";
    ID_TO_TYPENAME[KAKU] = "KAKU";
    ID_TO_TYPENAME[GYOKU] = "GYOKU";
}


Type typeutil::getType(string s) {
    return TYPENAME_TO_ID[s];
}


string typeutil::getStr(Type t) {
    return ID_TO_TYPENAME[t];
}

