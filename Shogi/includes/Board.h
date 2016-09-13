/*
 * Board.h
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */

#ifndef BOARD_H_
#define BOARD_H_

#include<iostream>
#include<vector>
#include"Constants.h"
#include"Point.h"
#include"Piece.h"
#include"Move.h"


class Board {
private:
    bool isSente;
    Piece board[SIZE][SIZE];
    std::vector<Piece> sentePiece;
    std::vector<Piece> gotePiece;

public:
    Board();

    bool getIsSente();
    void setIsSente(bool _isSente);
    void setPiece(int r, int c, const Piece &p);
    Piece getPiece(int r, int c);
    void addPiece(bool _isSente, const Piece &p);
    std::vector<Piece> getSentePiece();
    std::vector<Piece> getGotePiece();
    void eraseSentePiece(int idx);
    void eraseGotePiece(int idx);

    bool isInCheck(bool isGetSente);

    // 動かしうる次の局面のリストを返す
    // リストが空かどうかで勝ち負け判定が可能
    std::vector<std::pair<Board, Move> > listNextState();

    // (r, c) にある駒を動かす場合の、次の移動先を返す
    std::vector<std::pair<Piece, Point> > listNextMove(int r, int c);

    bool isValidMovement(int r, int c, Piece piece);
    bool isValidPlacement(int r, int c, Piece piece);
    bool isInside(int r, int c);
    bool canMoveTo(int nr, int nc, Piece &piece);
    bool operator< (const Board &opp) const;
    friend std::istream& operator>>(std::istream& in, Board& b);

    // 盤面の状態を、人間にとって見やすいフォーマットで出力
    //
    // 出力例:
    // 後手の持ち駒: 金  v
    // +------+------+------+
    // |      |      |玉  v|
    // +------+------+------+
    // |      |      |      |
    // +------+------+------+
    // |玉  ^|      |      |
    // +------+------+------+
    // 先手の持ち駒: 金  ^
    void print();

    // 盤面の状態を、プログラムが読み込めるようにフォーマットして出力
    //
    // [盤上にある先手の駒の数]
    // 先手の駒の種類_1 行_1 列_1 (NARI or HUNARI)_1
    // 先手の駒の種類_2 行_2 列_2 (NARI or HUNARI)_2
    // ...
    // [盤上にある後手の駒の数]
    // 後手の駒の種類_1 行_1 列_1 (NARI or HUNARI)_1
    // 後手の駒の種類_2 行_2 列_2 (NARI or HUNARI)_2
    // ...
    // [先手の持駒の数]
    // 先手の持駒の種類_1
    // 先手の持駒の種類_2
    // ...
    // [後手の持駒の数]
    // 後手の持駒の種類_1
    // 後手の持駒の種類_2
    // ...
    std::string format();
};

#endif /* BOARD_H_ */
