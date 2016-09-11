/*
 * Game.h
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */

#ifndef GAME_H_
#define GAME_H_

#include<map>
#include"Board.h"

class Game {
private:
    Board board;

    // state[局面] := 整数n
    //
    //   (i)   n = 0 の場合: 勝ち負け不明 (まだ訪れてないため)
    //   (ii)  n > 0 の場合:  n ターンで先手の勝ち (後手の負けの局面で n =  1 とする)
    //   (iii) n < 0 の場合: -n ターンで後手の勝ち (先手の負けの局面で n = -1 とする)
    std::map<Board, int> state;
    std::map<Board, Move> nextMove;


public:
    void init(char filename[]);
    std::map<Board, int> getState();
    void generateAllState();
    void bfs(Board &init);
};


#endif /* GAME_H_ */
