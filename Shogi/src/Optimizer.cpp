/*
 * Optimizer.cpp
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */

#include<iostream>
#include<string>
#include<map>
#include<fstream>
#include<climits>
#include"Constants.h"
#include"TypeUtil.h"
#include"Board.h"
#include"Optimizer.h"
using namespace std;


Optimizer::Optimizer() {}


Optimizer::Optimizer(char *_filename, const map<Board, int> &_state) {
    filename = (string)_filename + ".out";
    state = _state;
}


void Optimizer::optimize() {
    for (int loop = 0; loop < LOOP; ++loop) {
        for (map<Board, int>::iterator it = state.begin(); it != state.end(); ++it) {
            Board board = it->first;

            vector<pair<Board, Move> > nextList = board.listNextState();

            // 動けない場合、負けの局面
            if (nextList.empty()) {
                state[board] = board.getIsSente() ? -1 : 1;
                continue;
            }

            // 先手後手ともに、自分が勝つ手順があれば最短手数で勝つ手を指し、
            // どうしても勝てなければ最長手数で負ける手を指す

            // 先手が参照
            int minNegative = INT_MAX;
            int minPositive = INT_MAX;
            Move minNegativeMove;
            Move minPositiveMove;

            // 後手が参照
            int maxNegative = INT_MIN;
            int maxPositive = INT_MIN;
            Move maxNegativeMove;
            Move maxPositiveMove;

            for (int i = 0; i < nextList.size(); ++i) {
                int v = state[nextList[i].first];
                if (v < 0) {
                    if (minNegative > v) {
                        minNegative = v;
                        minNegativeMove = nextList[i].second;
                    }
                    if (maxNegative < v) {
                        maxNegative = v;
                        maxNegativeMove = nextList[i].second;
                    }
                }
                else if (v > 0) {
                    if (minPositive > v) {
                        minPositive = v;
                        minPositiveMove = nextList[i].second;
                    }
                    if (maxPositive < v) {
                        maxPositive = v;
                        maxPositiveMove = nextList[i].second;
                    }
                }
            }

            if (board.getIsSente()) {
                if (minPositive != INT_MAX) {
                    state[board] = minPositive + 1;
                    nextMove[board] = minPositiveMove;
                }
                else if (minNegative != INT_MAX) {
                    state[board] = minNegative - 1;
                    nextMove[board] = minNegativeMove;
                }
            }
            else {
                if (maxNegative != INT_MIN) {
                    state[board] = maxNegative - 1;
                    nextMove[board] = maxNegativeMove;
                }
                else if (maxPositive != INT_MIN) {
                    state[board] = maxPositive + 1;
                    nextMove[board] = maxPositiveMove;
                }
            }
        }
    }
}

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
void Optimizer::output() {
    ofstream fout;
    fout.open(filename.c_str());

    fout << nextMove.size() << endl;
    for (map<Board, Move>::iterator it = nextMove.begin(); it != nextMove.end(); ++it) {
        Board board = it->first;
        Move move = it->second;

        fout << board.format();
        if (move.isUtsu) {
            fout << "UTSU" << endl;
            fout << typeutil::getStr(move.utsuPiece.getType()) << ' ';
            fout << move.utsuTo.r << ' ' << move.utsuTo.c << endl;
        }
        else {
            fout << "MOVE" << endl;
            fout << move.moveFrom.r << ' ' << move.moveFrom.c << ' ';
            fout << move.moveTo.r << ' ' << move.moveTo.c << ' ';
            fout << (move.utsuPiece.getIsNaru() ? "NARI" : "HUNARI") << endl;
        }
    }
    fout.close();
}
