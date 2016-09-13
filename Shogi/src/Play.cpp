/*
 * Play.cpp
 *
 *  Created on: 2016/09/08
 *      Author: Ben
 */

#include<string>
#include<algorithm>
#include<fstream>
#include<cstdlib>
#include"Board.h"
#include"Play.h"
using namespace std;


void Play::init(char filename[], bool _isPlayerSente) {

    isPlayerSente = _isPlayerSente;

    string initBoardFile = filename;
    string nextMoveFile = (string)filename + ".out";

    cout << initBoardFile << " を読み込みます" << endl;

    ifstream fin;
    fin.open(initBoardFile.c_str());
    if (!fin) {
        cerr << "ファイル " << initBoardFile << " が開けません" << endl;
        exit(1);
    }
    fin >> initBoard;
    fin.close();

    cout << nextMoveFile << " を読み込みます" << endl;

    fin.open(nextMoveFile.c_str());
    int nMove;
    fin >> nMove;
    for (int i = 0; i < nMove; ++i) {
        Board board;
        Move move;
        fin >> board;
        fin >> move;
        if (move.isUtsu) {
            move.setUtsuPiece(Piece(move.utsuPiece.getType(), board.getIsSente(), false));
        }
        else {
            move.setMovedPiece(board.getPiece(move.moveFrom.r, move.moveFrom.c));
        }
        nextMove[board] = move;
    }
    fin.close();
}


void Play::play() {

    Board current = initBoard;

    while (1) {
        // 現在の盤面の表示
        current.print();

        if (current.listNextState().empty()) {
            if (current.getIsSente() == isPlayerSente) {
                cout << "あなたの負けです" << endl;
            }
            else {
                cout << "あなたの勝ちです" << endl;
            }
            break;
        }

        // 人間の手番の場合
        if (current.getIsSente() == isPlayerSente) {

            while (1) {
                printf("人間の手番です\n");

                // 手の読み込み
                Move move;
                cin >> move;

                if (move.isUtsu) {
                    move.setUtsuPiece(Piece(move.utsuPiece.getType(), current.getIsSente(), false));
                }
                else {
                    move.setMovedPiece(Piece(current.getPiece(move.moveFrom.r, move.moveFrom.c).getType(), current.getIsSente(), move.movedPiece.getIsNaru()));
                }

                // 駒を打つ場合
                if (move.isUtsu) {
                    int idx = -1;
                    Piece toPut;

                    vector<Piece> pieces = current.getIsSente() ? current.getSentePiece() : current.getGotePiece();
                    for (int i = 0; i < pieces.size(); ++i) {
                        if (pieces[i].getType() == move.utsuPiece.getType()) {
                            toPut = pieces[i];
                            idx = i;
                            break;
                        }
                    }

                    if (idx == -1) {
                        printf("指定した持ち駒がありません\n");
                        continue;
                    }

                    if (current.getPiece(move.utsuTo.r, move.utsuTo.c).getType() != BLANK) {
                        printf("指定した位置に駒があります\n");
                        continue;
                    }

                    if (!current.isValidPlacement(move.utsuTo.r, move.utsuTo.c, toPut)) {
                        printf("指定した位置には置けません\n");
                        continue;
                    }

                    Board next = current;

                    // 先手/後手の入れ替え
                    next.setIsSente(!next.getIsSente());

                    // 駒の配置
                    next.setPiece(move.utsuTo.r, move.utsuTo.c, toPut);

                    // 持駒から削除
                    if (current.getIsSente()) {
                        next.eraseSentePiece(idx);
                    }
                    else {
                        next.eraseGotePiece(idx);
                    }

                    // 打った後で玉が取られる場合は打たない
                    if (next.isInCheck(current.getIsSente())) {
                        printf("玉がとられます\n");
                        continue;
                    }

                    // 打った駒が歩で相手が負けになる場合、打ち歩詰めになるので打たない
                    if (toPut.getType() == HU && next.isInCheck(!current.getIsSente()) && next.listNextState().empty()) {
                        printf("打ち歩詰めです\n");
                        continue;
                    }

                    // 次の手番へ
                    current = next;
                    break;
                }
                // 駒を動かす場合
                else {
                    // 空欄は動かせない
                    if (current.getPiece(move.moveFrom.r, move.moveFrom.c).getType() == BLANK) {
                        printf("(%d, %d) に駒はありません。\n", move.moveFrom.r, move.moveFrom.c);
                        continue;
                    }

                    // 相手の駒は動かせない
                    if (current.getPiece(move.moveFrom.r, move.moveFrom.c).getIsSente() != current.getIsSente()) {
                        printf("(%d, %d) は相手の駒です。\n", move.moveFrom.r, move.moveFrom.c);
                        continue;
                    }

                    // 異常な動かし方
                    if (!current.isValidMovement(move.moveTo.r, move.moveTo.c, move.movedPiece)) {
                        printf("そのような動かし方はできません。\n");
                        continue;
                    }

                    Board next = current;

                    // 先手/後手の入れ替え
                    next.setIsSente(!next.getIsSente());

                    // 駒を (moveFrom.r, moveFrom.c) から (moveTo.r, moveTo.c) に動かす
                    // 相手の駒を取る
                    if (current.getPiece(move.moveTo.r, move.moveTo.c).getType() != BLANK) {
                        if (current.getIsSente()) {
                            next.addPiece(true, Piece(current.getPiece(move.moveTo.r, move.moveTo.c).getType(), true, false));
                        }
                        else {
                            next.addPiece(false, Piece(current.getPiece(move.moveTo.r, move.moveTo.c).getType(), false, false));
                        }
                    }
                    next.setPiece(move.moveTo.r, move.moveTo.c, move.movedPiece);
                    next.setPiece(move.moveFrom.r, move.moveFrom.c, Piece(BLANK, false, false));

                    // 動かした後で玉が取られる場合は動かさない
                    if (next.isInCheck(current.getIsSente())) {
                        printf("玉が取られます。\n");
                        continue;
                    }

                    // 次の手番へ
                    current = next;

                    break;
                }
            }
        }
        // コンピュータの手番の場合
        else {
            printf("コンピュータの手番です\n");

            Move move = nextMove[current];

            // 駒を打つ場合
            if (move.isUtsu) {
                int idx = -1;
                Piece toPut;

                vector<Piece> pieces = current.getIsSente() ? current.getSentePiece() : current.getGotePiece();
                for (int i = 0; i < pieces.size(); ++i) {
                    if (pieces[i].getType() == move.utsuPiece.getType()) {
                        toPut = pieces[i];
                        idx = i;
                        break;
                    }
                }

                Board next = current;

                // 先手/後手の入れ替え
                next.setIsSente(!next.getIsSente());

                // 駒の配置
                next.setPiece(move.utsuTo.r, move.utsuTo.c, toPut);

                // 持駒から削除
                if (current.getIsSente()) {
                    next.eraseSentePiece(idx);
                }
                else {
                    next.eraseGotePiece(idx);
                }

                // 次の手番へ
                current = next;
            }
            // 駒を動かす場合
            else {

                Board next = current;

                // 先手/後手の入れ替え
                next.setIsSente(!next.getIsSente());

                // 駒を (moveFrom.r, moveFrom.c) から (moveTo.r, moveTo.c) に動かす
                // 相手の駒を取る
                if (current.getPiece(move.moveTo.r, move.moveTo.c).getType() != BLANK) {
                    if (current.getIsSente()) {
                        next.addPiece(true, Piece(current.getPiece(move.moveTo.r, move.moveTo.c).getType(), true, false));
                    }
                    else {
                        next.addPiece(false, Piece(current.getPiece(move.moveTo.r, move.moveTo.c).getType(), false, false));
                    }
                }

                next.setPiece(move.moveTo.r, move.moveTo.c, move.movedPiece);
                next.setPiece(move.moveFrom.r, move.moveFrom.c, Piece(BLANK, false, false));

                // 動かした後で玉が取られる場合は動かさない
                if (next.isInCheck(current.getIsSente())) {
                    printf("玉が取られます。\n");
                    continue;
                }

                // 次の手番へ
                current = next;
            }
        }
    }
}
