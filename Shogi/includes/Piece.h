/*
 * Piece.h
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */

#ifndef PIECE_H_
#define PIECE_H_

#include<vector>
#include"Constants.h"
#include"Point.h"


class Piece {
private:
    Type type;
    bool isSente; // 先手の駒かどうか
    bool isNaru;  // 成り駒かどうか

public:
    Piece();
    Piece(Type _type, bool _isSente, bool _isNaru);

    Type getType();
    bool getIsSente();
    bool getIsNaru();
    void setIsNaru(bool _isNaru);

    std::vector<Point> getDRDLList();
    std::string toString();

    bool operator== (const Piece &opp) const;
    bool operator!= (const Piece &opp) const;
    bool operator< (const Piece &opp) const;
};



#endif /* PIECE_H_ */
