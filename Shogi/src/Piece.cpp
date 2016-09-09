/*
 * Piece.cpp
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */
#include<iostream>
#include<vector>
#include<cassert>
#include"Constants.h"
#include"Point.h"
#include"Piece.h"
using namespace std;

Piece::Piece(){};


Piece::Piece(Type _type, bool _isSente, bool _isNaru) {
    type = _type;
    isSente = _isSente;
    isNaru = _isNaru;
}


Type Piece::getType() {
    return type;
}


bool Piece::getIsSente() {
    return isSente;
}


bool Piece::getIsNaru() {
    return isNaru;
}


void Piece::setIsNaru(bool _isNaru) {
    isNaru = _isNaru;
}


vector<Point> Piece::getDRDLList() {
    vector<Point> ret;

    if (type == KIN ||
        (isNaru && (type == HU || type == KYOSYA || type == KEIMA || type == GIN))) {

        static int dr[] = {-1, -1, -1,  0, 0, -1};
        static int dc[] = {-1,  0,  1, -1, 1,  0};
        for (int i = 0; i < 6; ++i) {
            ret.push_back(Point(dr[i], dc[i]));
        }
    }
    else if (type == HU) {
        ret.push_back(Point(-1, 0));
    }
    else if (type == KYOSYA) {
        // 香車は他の駒を飛び越えて移動することができないので、利用には注意が必要
        for (int i = 1; i < SIZE; ++i) {
            ret.push_back(Point(-i, 0));
        }
    }
    else if (type == KEIMA) {
        static int dr[] = {-2, -2};
        static int dc[] = {-1,  1};
        for (int i = 0; i < 2; ++i) {
            ret.push_back(Point(dr[i], dc[i]));
        }
    }
    else if (type == GIN) {
        static int dr[] = {-1, -1, -1,  1, 1};
        static int dc[] = {-1,  0,  1, -1, 1};
        for (int i = 0; i < 5; ++i) {
            ret.push_back(Point(dr[i], dc[i]));
        }
    }
    else if (type == HISYA) {
        // 飛車は他の駒を飛び越えて移動することができないので、利用には注意が必要
        for (int i = 1; i < SIZE; ++i) {
            ret.push_back(Point(-i,  0));
            ret.push_back(Point( 0,  i));
            ret.push_back(Point( i,  0));
            ret.push_back(Point( 0, -i));
        }
        if (isNaru) {
            ret.push_back(Point(-1, -1));
            ret.push_back(Point(-1,  1));
            ret.push_back(Point( 1, -1));
            ret.push_back(Point( 1,  1));
        }
    }
    else if (type == KAKU) {
        // 角は他の駒を飛び越えて移動することができないので、利用には注意が必要
        for (int i = 1; i < SIZE; ++i) {
            ret.push_back(Point(-i, -i));
            ret.push_back(Point(-i,  i));
            ret.push_back(Point( i,  i));
            ret.push_back(Point( i, -i));
        }
        if (isNaru) {
            ret.push_back(Point(-1,  0));
            ret.push_back(Point( 0,  1));
            ret.push_back(Point( 1,  0));
            ret.push_back(Point( 0, -1));
        }
    }
    else if (type == GYOKU) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                ret.push_back(Point(dr, dc));
            }
        }
    }
    else {
        cerr << "動かそうとしている駒が不正です" << endl;
    }

    if (!isSente) {
        for (int i = 0; i < ret.size(); ++i) {
            ret[i].r *= -1;
        }
    }

    return ret;
}


string Piece::toString() {
    string ret;

    switch (type) {
        case BLANK: ret = "      "; break;
        case HU: ret = (!isNaru ? "歩  " : "と  "); break;
        case KYOSYA: ret = (!isNaru ? "香  " : "成香"); break;
        case KEIMA: ret = (!isNaru ? "桂  " : "成桂"); break;
        case GIN: ret = (!isNaru ? "銀  " : "成銀"); break;
        case KIN: ret = "金  "; break;
        case HISYA: ret = (!isNaru ? "飛  " : "龍  "); break;
        case KAKU: ret = (!isNaru ? "角  " : "馬  "); break;
        case GYOKU: ret = "玉  "; break;
        default: cerr<<"不正な駒です"<<endl; assert(false); break;
    }

    if (type != BLANK) {
        ret += (isSente ? " ^" : " v");
    }

    return ret;
}


bool Piece::operator== (const Piece &opp) const {
    if (type != opp.type) return false;
    if (isSente != opp.isSente) return false;
    return isNaru == opp.isNaru;
}


bool Piece::operator!= (const Piece &opp) const {
    if (*this == opp) return false;
    return true;
}


bool Piece::operator< (const Piece &opp) const {
    if (type != opp.type) return type < opp.type;
    if (isSente != opp.isSente) return isSente < opp.isSente;
    return isNaru < opp.isNaru;
}

