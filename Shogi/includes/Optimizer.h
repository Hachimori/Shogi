/*
 * Optimizer.h
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */

#ifndef OPTIMIZER_H_
#define OPTIMIZER_H_

#include<map>
#include"Board.h"
#include"Move.h"

class Optimizer {
private:
    std::string filename;

    // state[局面] := 整数n
    //
    //   (i)   n = 0 の場合: 勝ち負け不明 (まだ訪れてないため)
    //   (ii)  n > 0 の場合:  n ターンで先手の勝ち (後手の負けの局面で n =  1 とする)
    //   (iii) n < 0 の場合: -n ターンで後手の勝ち (先手の負けの局面で n = -1 とする)
    std::map<Board, int> state;
    std::map<Board, Move> nextMove;

public:
    Optimizer();
    Optimizer(char *_filename, const std::map<Board, int> &_state);

    void optimize();

    // 各局面の最善手を出力
    //
    // フォーマット:
    // [局面の数]
    // [局面のフォーマット_1]
    // [手のフォーマット_1]
    // [局面のフォーマット_2]
    // [手のフォーマット_2]
    // [局面のフォーマット_3]
    // [手のフォーマット_3]
    // ...
    //
    //
    // 局面のフォーマット:
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
    //
    //
    //
    // 最善手のフォーマット:
    //
    // (i) 打つ場合:
    // UTSU
    // [駒の種類] [行番号] [列番号]
    //
    // (ii) 盤上の駒を動かす場合:
    // MOVE
    // [移動元の行番号] [移動元の列番号] [移動先の行番号] [移動先の列番号] (NARI or HUNARI)
    void output();
};


#endif /* OPTIMIZER_H_ */
