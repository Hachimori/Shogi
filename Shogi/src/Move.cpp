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
#include"Board.h"
using namespace std;


void Move::setUtsuPiece(const Piece &p) {
    utsuPiece = p;
}


void Move::setMovedPiece(const Piece &p) {
    movedPiece = p;
}


istream& operator>>(istream &in, Move &move) {

    string moveType;
    in >> moveType;

    if (moveType == "UTSU") {
        string pieceType;
        int r, c;
        in >> pieceType >> r >> c;

        move.isUtsu = true;
        move.utsuPiece = Piece(typeutil::getType(pieceType), false, false);
        move.utsuTo = Point(r, c);
    }
    else {
        int moveFromR, moveFromC;
        int moveToR, moveToC;
        string nari;
        in >> moveFromR >> moveFromC >> moveToR >> moveToC >> nari;

        move.isUtsu = false;
        move.moveFrom = Point(moveFromR, moveFromC);
        move.moveTo = Point(moveToR, moveToC);
        move.movedPiece = Piece(BLANK, false, (nari == "NARI"));
    }
}
