/*
 * Move.h
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */

#ifndef MOVE_H_
#define MOVE_H_

#include"Point.h"
#include"Piece.h"


class Move {
public:
    // 持駒の中から打つ場合は true, 盤上の駒を動かす場合は false
    bool isUtsu;

    // (i) 持駒の中から打たれた場合
    //     - utsuTo: 打たれた場所を格納
    //     - utsuPiece: 打った駒を格納
    Point utsuTo;
    Piece utsuPiece;

    // (ii) 盤上の駒を動かした場合
    //     - moveFrom: 動かした駒の移動元の場所を格納
    //     - moveTo: 動かした駒の移動先の場所を格納
    //     - movedPiece: 動かした駒を格納 (成/不成 を参照するのに利用)
    Point moveFrom;
    Point moveTo;
    Piece movedPiece;

    void setUtsuPiece(const Piece &p);
    void setMovedPiece(const Piece &p);

    friend std::istream& operator>>(std::istream& in, Move& m);
};


#endif /* MOVE_H_ */
