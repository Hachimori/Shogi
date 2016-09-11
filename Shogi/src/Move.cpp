/*
 * Move.cpp
 *
 *  Created on: 2016/09/11
 *      Author: Ben
 */

#include<iostream>
#include<string>
#include"TypeUtil.h"
#include"Move.h"
using namespace std;


istream& operator>>(std::istream& in, Move& m) {

    string moveType;
    in >> moveType;

    if (moveType == "UTSU") {
        string pieceType;
        int r, c;
        in >> pieceType >> r >> c;

        m.isUtsu = true;
        // TODO: なおす
        m.utsuPiece = Piece(typeutil::getType(pieceType), false, false);
        m.utsuTo = Point(r, c);
    }
    else {
        int moveFromR, moveFromC;
        int moveToR, moveToC;
        string nari;
        in >> moveFromR >> moveFromC >> moveToR >> moveToC >> nari;

        m.isUtsu = false;
        m.moveFrom = Point(moveFromR, moveFromC);
        m.moveTo = Point(moveToR, moveToC);
        //m.utsuPiece =
        //fout << move.moveFrom.r << ' ' << move.moveFrom.c << ' ';
        //fout << move.moveTo.r << ' ' << move.moveTo.c << ' ';
        //fout << (move.utsuPiece.getIsNaru() ? "NARI" : "HUNARI") << endl;
    }
}
