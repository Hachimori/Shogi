//============================================================================
// Name        : Shogi.cpp
// Author      : Hachimori
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<iostream>
#include<cstring>
#include"TypeUtil.h"
#include"Game.h"
#include"Optimizer.h"
using namespace std;


int main(int argc, char *argv[]) {
    typeutil::init();

    if (!((argc == 3 && strcmp(argv[1], "optimize") == 0) ||
          (argc == 4 && strcmp(argv[1], "play") == 0 && (strcmp(argv[3], "SENTE") == 0 || strcmp(argv[4], "GOTE") == 0)))) {
        cerr << "使い方:" << endl;
        cerr << " ./a.exe optimize filename" << endl;
        cerr << " ./a.exe play filename (SENTE | GOTE)" << endl;
        return 1;
    }

    // 思考ロジックの作成
    if (strcmp(argv[1], "optimize") == 0) {
        Game game;
        game.init(argv[2]);
        game.generateAllState();

        Optimizer optimizer = Optimizer(argv[2], game.getState());
        optimizer.optimize();
        optimizer.output();
    }

    return 0;

    // ゲームのプレイ
    if (strcmp(argv[1], "play") == 0) {
        Play play;
        // play.init(argv[2], strcmp(argv[3], "SENTE") == 0);
    }
}
