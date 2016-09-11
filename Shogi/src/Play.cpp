/*
 * Play.cpp
 *
 *  Created on: 2016/09/08
 *      Author: Ben
 */

#include<string>
#include<fstream>
#include<cstdlib>
#include"Board.h"
#include"Play.h"
using namespace std;


void Play::init(char filename[], bool _isPlayerSente) {

    isPlayerSente = _isPlayerSente;

    string initBoard = filename;
    string moveList = (string)filename + ".out";

    ifstream fin;
    fin.open(initBoard.c_str());
    if (!fin) {
        cerr << "ファイル " << initBoard << " が開けません" << endl;
        exit(1);
    }

    int nMove;
    fin >> nMove;
    for (int i = 0; i < nMove; ++i) {
        Board board;
        Move move;
        fin >> board;
        fin >> move;
    }
    /*
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
    */
    fin.close();
}


void Play::play() {

}
