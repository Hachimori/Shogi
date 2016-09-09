/*
 * Board.cpp
 *
 *  Created on: 2016/09/07
 *      Author: Ben
 */
#include<iostream>
#include<sstream>
#include<vector>
#include<algorithm>
#include<cassert>
#include<cstdio>
#include"Constants.h"
#include"Point.h"
#include"Piece.h"
#include"Board.h"
#include"Move.h"
#include"TypeUtil.h"
using namespace std;


bool Board::getIsSente() {
    return isSente;
}


void Board::setIsSente(bool _isSente) {
    isSente = _isSente;
}


void Board::setPiece(int r, int c, const Piece &p) {
    assert(0 <= r && r < SIZE);
    assert(0 <= c && c < SIZE);
    board[r][c] = p;
}


Piece Board::getPiece(int r, int c) {
    assert(0 <= r && r < SIZE);
    assert(0 <= c && c < SIZE);
    return board[r][c];
}


void Board::addPiece(bool _isSente, const Piece &p) {
    if (_isSente) {
        sentePiece.push_back(p);
        sort(sentePiece.begin(), sentePiece.end());
    }
    else {
        gotePiece.push_back(p);
        sort(gotePiece.begin(), gotePiece.end());
    }
}


bool Board::isInCheck(bool isGetSente) {

    // 相手が動かせる駒を探す
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {

            // 空欄は動かせない
            if (board[r][c].getType() == BLANK) {
                continue;
            }

            // 自分の駒は考えない
            if (board[r][c].getIsSente() == isGetSente) {
                continue;
            }

            // 相手が動かせる駒の動きの候補
            vector< pair<Piece, Point> > oppMoveList = listNextMove(r, c);

            for (int i = 0; i < oppMoveList.size(); ++i) {
                Point &moveToPt = oppMoveList[i].second;
                Piece &moveToPiece = board[moveToPt.r][moveToPt.c];

                // 移動先のマスにある駒が自分の玉の場合
                if (moveToPiece.getType() == GYOKU && moveToPiece.getIsSente() == isGetSente) {
                    return true;
                }
            }
        }
    }

    return false;
}


// 動かしうる次の局面のリストを返す
// リストが空かどうかで勝ち負け判定が可能
vector<pair<Board, Move> > Board::listNextState() {

    vector<pair<Board, Move> > ret;

    // 駒を動かす
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            // 空欄は動かせない
            if (board[r][c].getType() == BLANK) {
                continue;
            }

            // 相手の駒は動かせない
            if (board[r][c].getIsSente() != isSente) {
                continue;
            }

            // 移動先の候補 & 成/不成 の選択
            vector< pair<Piece, Point> > nextMove = listNextMove(r, c);

            for (int i = 0; i < nextMove.size(); ++i) {
                Board next = *this;

                // 先手/後手の入れ替え
                next.isSente = !next.isSente;

                // 移動先の場所
                int nr = nextMove[i].second.r;
                int nc = nextMove[i].second.c;

                // 駒を (r, c) から (nr, nc) に動かす
                // 相手の駒を取る
                if (board[nr][nc].getType() != BLANK) {
                    if (isSente) {
                        next.sentePiece.push_back(Piece(board[nr][nc].getType(), true, false));
                        sort(next.sentePiece.begin(), next.sentePiece.end());
                    }
                    else {
                        next.gotePiece.push_back(Piece(board[nr][nc].getType(), false, false));
                        sort(next.gotePiece.begin(), next.gotePiece.end());
                    }
                }
                next.board[nr][nc] = nextMove[i].first;
                next.board[r][c] = Piece(BLANK, false, false);

                // 動かした後で玉が取られる場合は動かさない
                if (next.isInCheck(isSente)) {
                    continue;
                }

                // どのように動かしたかをセット
                Move move;
                move.isUtsu = false;
                move.moveFrom = Point(r, c);
                move.moveTo = Point(nr, nc);
                move.movedPiece = nextMove[i].first;

                // 次の状態の候補を追加
                ret.push_back(make_pair(next, move));
            }
        }
    }

    // 駒を打つ
    for (int i = 0; i < (isSente ? sentePiece.size() : gotePiece.size()); ++i) {
        Piece &toPut = isSente ? sentePiece[i] : gotePiece[i];

        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {

                if (!(board[r][c].getType() == BLANK && isValidPlacement(r, c, toPut))) {
                    continue;
                }

                Board next = *this;

                // 先手/後手の入れ替え
                next.isSente = !next.isSente;

                // 駒の配置
                next.board[r][c] = toPut;

                // 持駒から削除
                if (isSente) {
                    next.sentePiece.erase(next.sentePiece.begin() + i);
                }
                else {
                    next.gotePiece.erase(next.gotePiece.begin() + i);
                }

                // 打った後で玉が取られる場合は打たない
                if (next.isInCheck(isSente)) {
                    continue;
                }

                // 打った駒が歩で相手が負けになる場合、打ち歩詰めになるので打たない
                if (board[r][c].getType() == HU && next.isInCheck(!isSente) && next.listNextState().empty()) {
                    continue;
                }

                // どのように打ったかをセット
                Move move;
                move.isUtsu = true;
                move.utsuTo = Point(r, c);
                move.utsuPiece = toPut;

                // 次の状態の候補を追加
                ret.push_back(make_pair(next, move));
            }
        }
    }

    return ret;
}


// (r, c) にある駒を動かす場合の、次の移動先を返す
vector< pair<Piece, Point> > Board::listNextMove(int r, int c) {
    Piece &piece = board[r][c];

    if (piece.getType() == BLANK) {
        cerr << "空白です" << endl;
        return vector< pair<Piece, Point> >();
    }

    if (!isValidPlacement(r, c, piece)) {
        cerr << "異常な置き方です" << endl;
        return vector< pair<Piece, Point> >();
    }

    vector< pair<Piece, Point> > ret;

    if ((piece.getType() == KYOSYA && !piece.getIsNaru()) ||
        piece.getType() == HISYA ||
        piece.getType() == KAKU) {
        // 香車不成, 飛車, 角の場合

        vector<Point> moveTo;

        if (piece.getType() == KYOSYA) {
            for (int dr = 1; dr < SIZE; ++dr) {
                int nr = r + (piece.getIsSente() ? -dr : dr);
                int nc = c;

                // 動かせない場合 (盤外に出てしまう, 自分の駒を取ってしまうなどの理由で)
                if (!canMoveTo(nr, nc, piece)) {
                    break;
                }

                // 移動先の候補に追加
                moveTo.push_back(Point(nr, nc));

                // 相手の駒であった場合、それより先に進まない
                if (board[nr][nc].getType() != BLANK) {
                    break;
                }
            }
        }
        else if (piece.getType() == HISYA) {
            static int dr[] = {-1, 0, 1, 0};
            static int dc[] = {0, 1, -1, 0};

            for (int i = 0; i < 4; ++i) {
                for (int mul = 1; mul < SIZE; ++mul) {
                    int nr = r + dr[i] * mul;
                    int nc = c + dc[i] * mul;

                    // 動かせない (盤外に出てしまう, 自分の駒を取ってしまうなどの理由で)
                    if (!canMoveTo(nr, nc, piece)) {
                        break;
                    }

                    // 移動先の候補に追加
                    moveTo.push_back(Point(nr, nc));

                    // 相手の駒であった場合、それより先に進まない
                    if (board[nr][nc].getType() != BLANK) {
                        break;
                    }
                }
            }

            // 飛成の場合
            if (piece.getIsNaru()) {
                static int dr[] = {-1, 0, 1, 0};
                static int dc[] = {0, 1, -1, 0};

                for (int i = 0; i < 4; ++i) {
                    int nr = r + dr[i];
                    int nc = c + dc[i];

                    // 動かせない (盤外に出てしまう, 自分の駒を取ってしまうなどの理由で)
                    if (!canMoveTo(nr, nc, piece)) {
                        continue;
                    }

                    // 移動先の候補に追加
                    moveTo.push_back(Point(nr, nc));
                }
            }
        }
        else {
            static int dr[] = {-1, -1, 1, 1};
            static int dc[] = {-1, 1, 1, -1};

            for (int i = 0; i < 4; ++i) {
                for (int mul = 1; mul < SIZE; ++mul) {
                    int nr = r + dr[i] * mul;
                    int nc = c + dc[i] * mul;

                    // 動かせない (盤外に出てしまう, 自分の駒を取ってしまうなどの理由で)
                    if (!canMoveTo(nr, nc, piece)) {
                        break;
                    }

                    // 移動先の候補に追加
                    moveTo.push_back(Point(nr, nc));

                    // 相手の駒であった場合、それより先に進まない
                    if (board[nr][nc].getType() != BLANK) {
                        break;
                    }
                }
            }

            // 角成の場合
            if (piece.getIsNaru()) {
                static int dr[] = {-1, -1, 1, 1};
                static int dc[] = {-1, 1, 1, -1};

                for (int i = 0; i < 4; ++i) {
                    int nr = r + dr[i];
                    int nc = c + dc[i];

                    // 動かせない (盤外に出てしまう, 自分の駒を取ってしまうなどの理由で)
                    if (!canMoveTo(nr, nc, piece)) {
                        continue;
                    }

                    // 移動先の候補に追加
                    moveTo.push_back(Point(nr, nc));
                }
            }
        }

        // 不成で移動させるか、成で移動させるかを決める
        for (int i = 0; i < moveTo.size(); ++i) {
            int nr = moveTo[i].r;
            int nc = moveTo[i].c;

            // 不成で移動させる (既に成っている駒を動かす場合でもここで対処している)
            // 不成の駒の移動先の位置が妥当であるかどうかをチェックする
            if (isValidMovement(nr, nc, piece)) {
                ret.push_back(make_pair(piece, Point(nr, nc)));
            }

            // 成れるかのチェック
            // 成れない位置にいる場合、成れない
            if (!( piece.getIsSente() && (r == 0        || nr == 0       )) &&
                !(!piece.getIsSente() && (r == SIZE - 1 || nr == SIZE - 1))) {
                continue;
            }

            // 既に成っている場合、成れない
            if (piece.getIsNaru()) {
                continue;
            }

            // 成る
            piece.setIsNaru(true);
            ret.push_back(make_pair(piece, Point(nr, nc)));
            piece.setIsNaru(false);
        }
    }
    else {
        // 香車不成, 飛車, 角以外の場合
        vector<Point> drdlList = piece.getDRDLList();
        for (int i = 0; i < drdlList.size(); ++i) {
            int nr = r + drdlList[i].r;
            int nc = c + drdlList[i].c;

            // 動かせない (盤外に出てしまう, 自分の駒を取ってしまうなどの理由で)
            if (!canMoveTo(nr, nc, piece)) {
                continue;
            }

            // 不成で移動させる (既に成っている駒を動かす場合でもここで対処している)
            // 不成の駒の移動先の位置が妥当であるかどうかをチェックする
            if (isValidMovement(nr, nc, piece)) {
                ret.push_back(make_pair(piece, Point(nr, nc)));
            }

            // 成れるかのチェック
            // 成れない位置にいる場合、成れない
            if (!( piece.getIsSente() && (r == 0        || nr == 0       )) &&
                !(!piece.getIsSente() && (r == SIZE - 1 || nr == SIZE - 1))) {
                continue;
            }

            // 既に成っている場合、成れない
            if (piece.getIsNaru()) {
                continue;
            }

            // 金か玉の場合、成れない
            if (piece.getType() == KIN || piece.getType() == GYOKU) {
                continue;
            }

            // 成る
            piece.setIsNaru(true);
            ret.push_back(make_pair(piece, Point(nr, nc)));
            piece.setIsNaru(false);
        }
    }

    return ret;
}


bool Board::isValidMovement(int r, int c, Piece piece) {
    // 歩の位置のチェック
    if (piece.getType() == HU && !piece.getIsNaru()) {
        if (piece.getIsSente()) {
            return r > 0;
        }
        else {
            return r < SIZE - 1;
        }
    }

    // 桂馬の位置のチェック
    if (piece.getType() == KEIMA && !piece.getIsNaru()) {
        if (piece.getIsSente()) {
            return r > 1;
        }
        else {
            return r < SIZE - 2;
        }
    }

    // 香車の位置のチェック
    if (piece.getType() == KYOSYA && !piece.getIsNaru()) {
        if (piece.getIsSente()) {
            return r > 0;
        }
        else {
            return r < SIZE - 1;
        }
    }

    return true;
}

bool Board::isValidPlacement(int r, int c, Piece piece) {
    // 二歩のチェック
    if (piece.getType() == HU && !piece.getIsNaru()) {
        for (int rr = 0; rr < SIZE; ++rr) {
            if (r == rr) continue;
            if (board[rr][c].getType() == HU && !board[rr][c].getIsNaru() && board[rr][c].getIsSente() == piece.getIsSente()) {
                return false;
            }
        }
    }
    // 歩, 桂馬, 香車の位置のチェック
    return isValidMovement(r, c, piece);
}


bool Board::isInside(int r, int c) {
    return 0 <= r && r < SIZE && 0 <= c && c < SIZE;
}


bool Board::canMoveTo(int nr, int nc, Piece &piece) {
    return isInside(nr, nc) && (board[nr][nc].getType() == BLANK || board[nr][nc].getIsSente() != piece.getIsSente());
}


bool Board::operator< (const Board &opp) const {
    if (isSente != opp.isSente) return isSente < opp.isSente;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] != opp.board[i][j]) {
                return board[i][j] < opp.board[i][j];
            }
        }
    }

    if (sentePiece != opp.sentePiece) return sentePiece < opp.sentePiece;
    return gotePiece < opp.gotePiece;
}


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
void Board::print() {

    // 後手の持駒
    printf("後手の持ち駒:");
    for (int i = 0; i < gotePiece.size(); ++i) {
        Piece &piece = gotePiece[i];
        printf("\x1b[36m");
        printf(" %s", piece.toString().c_str());
    }
    printf("\n");

    for (int i = 0; i < SIZE; ++i) {
        // 上辺
        printf("\x1b[39m");
        for (int j = 0; j < SIZE; ++j) {
            printf("+------");
        }
        printf("+\n");

        // 中身
        // (将棋では右から 1, 2, 3, ... となるので、逆順に表示する)
        for (int j = SIZE - 1; j >= 0; --j) {
            Piece &piece = board[i][j];

            printf("\x1b[39m");
            printf("|");

            if (piece.getIsSente()) {
                printf("\x1b[35m");
            }
            else {
                printf("\x1b[36m");
            }

            printf("%s", piece.toString().c_str());
        }
        printf("\x1b[39m");
        printf("|\n");
    }

    // 下辺
    printf("\x1b[39m");
    for (int j = 0; j < SIZE; ++j) {
        printf("+------");
    }
    printf("+\n");

    // 先手の持駒
    printf("先手の持ち駒:");
    for (int i = 0; i < sentePiece.size(); ++i) {
        Piece &piece = sentePiece[i];
        printf("\x1b[35m");
        printf(" %s", piece.toString().c_str());
    }
    printf("\n");

    // 文字の色を元に戻す
    printf("\x1b[39m");
}


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
string Board::format() {
    vector<pair<Piece, Point> > senteBoardPiece;
    vector<pair<Piece, Point> > goteBoardPiece;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j].getType() == BLANK) continue;
            if (board[i][j].getIsSente()) {
                senteBoardPiece.push_back(make_pair(board[i][j], Point(i, j)));
            }
            else {
                goteBoardPiece.push_back(make_pair(board[i][j], Point(i, j)));
            }
        }
    }

    ostringstream out;
    out << senteBoardPiece.size() << endl;
    for (int i = 0; i < senteBoardPiece.size(); ++i) {
        Piece &pi = senteBoardPiece[i].first;
        Point &pt = senteBoardPiece[i].second;
        out << typeutil::getStr(pi.getType()) << ' ' << pt.r << ' ' << pt.c << ' ' << (pi.getIsNaru() ? "NARI" : "HUNARI") << endl;
    }

    out << goteBoardPiece.size() << endl;
    for (int i = 0; i < goteBoardPiece.size(); ++i) {
        Piece &pi = goteBoardPiece[i].first;
        Point &pt = goteBoardPiece[i].second;
        out << typeutil::getStr(pi.getType()) << ' ' << pt.r << ' ' << pt.c << ' ' << (pi.getIsNaru() ? "NARI" : "HUNARI") << endl;
    }

    out << sentePiece.size() << endl;
    for (int i = 0; i < sentePiece.size(); ++i) {
        Piece &pi = sentePiece[i];
        out << typeutil::getStr(pi.getType()) << endl;
    }

    out << gotePiece.size() << endl;
    for (int i = 0; i < gotePiece.size(); ++i) {
        Piece &pi = gotePiece[i];
        out << typeutil::getStr(pi.getType()) << endl;
    }

    return out.str();
}
