/*
 * Game.cpp
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */
#include<iostream>
#include<fstream>
#include<map>
#include<queue>
#include<cstdlib>
#include"Constants.h"
#include"TypeUtil.h"
#include"Piece.h"
#include"Board.h"
#include"Game.h"
using namespace std;


void Game::init(char filename[]) {
    ifstream fin; fin.open(filename);
    if (!fin) {
        cerr << "ファイル " << filename << " が開けません" << endl;
        exit(1);
    }

    cout << filename << " を読み込みます" << endl;
    // 初期化
    board = Board();
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board.setPiece(i, j, Piece(BLANK, false, false));
        }
    }
    state.clear();

    // 先手の盤上の駒
    int nSenteBoardPiece;
    fin >> nSenteBoardPiece;

    for (int i = 0; i < nSenteBoardPiece; ++i) {
        string typeName;
        int r, c;
        fin >> typeName >> r >> c;
        cout << typeName << ' ' << r << ' ' << c << ' ' << typeutil::getType(typeName) << endl;
        board.setPiece(r, c, Piece(typeutil::getType(typeName), true, false));
    }

    // 後手の盤上の駒
    int nGoteBoardPiece;
    fin >> nGoteBoardPiece;

    for (int i = 0; i < nGoteBoardPiece; ++i) {
        string typeName;
        int r, c;
        fin >> typeName >> r >> c;
        board.setPiece(r, c, Piece(typeutil::getType(typeName), false, false));
    }

    // 先手の持駒
    int nSenteOwnPiece;
    fin >> nSenteOwnPiece;

    for (int i = 0; i < nSenteOwnPiece; ++i) {
        string typeName;
        fin >> typeName;
        board.addPiece(true, Piece(typeutil::getType(typeName), true, false));
    }

    // 後手の持駒
    int nGoteOwnPiece;
    fin >> nGoteOwnPiece;

    for (int i = 0; i < nGoteOwnPiece; ++i) {
        string typeName;
        fin >> typeName;
        board.addPiece(false, Piece(typeutil::getType(typeName), false, false));
    }

    fin.close();

    board.setIsSente(true);
}


map<Board, int> Game::getState() {
    return state;
}


void Game::generateAllState() {
    cout << "以下の局面を生成します" << endl;
    board.print();

    bfs(board);

    cout << "完了しました。状態数: " << state.size() << endl;
}


void Game::bfs(Board &init) {
    queue<pair<Board, int> > Q;

    state[init] = 0;
    Q.push(make_pair(init, 0));

    while (!Q.empty()) {
        pair<Board, int> curr = Q.front();
        Q.pop();

        vector<pair<Board, Move> > nextList = curr.first.listNextState();

        // 動けない場合は負け
        if (nextList.empty()) {
            continue;
        }

        for (int i = 0; i < nextList.size(); ++i) {
            if (state.count(nextList[i].first)) continue;
            state[nextList[i].first] = 0;
            Q.push(make_pair(nextList[i].first, curr.second + 1));
        }
    }
}

