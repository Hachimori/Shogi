/*
 * TypeUtil.h
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */

#ifndef TYPEUTIL_H_
#define TYPEUTIL_H_

#include<map>
#include"Constants.h"

namespace typeutil {
    static std::map<std::string, Type> TYPENAME_TO_ID;
    static std::map<Type, std::string> ID_TO_TYPENAME;

    void init();
    Type getType(std::string s);
    std::string getStr(Type t);
};


#endif /* TYPEUTIL_H_ */
