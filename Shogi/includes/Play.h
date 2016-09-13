/*
 * Play.h
 *
 *  Created on: 2016/09/08
 *      Author: Ben
 */

#ifndef PLAY_H_
#define PLAY_H_

class Play {
private:
    bool isPlayerSente;  // 人間は先手かどうか
    Board initBoard;

    // state[局面] := 整数n
    //
    //   (i)   n = 0 の場合: 勝ち負け不明 (まだ訪れてないため)
    //   (ii)  n > 0 の場合:  n ターンで先手の勝ち (後手の負けの局面で n =  1 とする)
    //   (iii) n < 0 の場合: -n ターンで後手の勝ち (先手の負けの局面で n = -1 とする)
    std::map<Board, Move> nextMove;

public:
    void init(char filename[], bool _isPlayerSente);
    void play();
};



#endif /* PLAY_H_ */
