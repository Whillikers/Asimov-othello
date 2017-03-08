#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <vector>
#include <utility>
#include <iostream>
#include "common.hpp"
using namespace std;

typedef pair<unsigned long long, unsigned long long> BoardNormalForm;

struct MoveResult {
    int x, y, flipped[9];

    MoveResult() {
        for (int i = 0; i < 9; i++) {
            flipped[i] = 0;
        }
    }

    void set(int dx, int dy, int c) {
        flipped[3*dy + 4 + dx] = c;
    }

    int get(int dx, int dy) {
        return flipped[3*dy + 4 + dx];
    }
};

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    bool occupied(int x, int y);
    void set(Side side, int x, int y);
    void flip(int x, int y);
    void clear(int x, int y);
    bool onBoard(int x, int y);

    Board rotate_90_ccw();
    Board flip_horizontal();
    BoardNormalForm to_num_form();

public:
    friend class Heuristic;

    Board();
    static Board empty();
    ~Board();
    //deprecate this to avoid allocations
    Board *copy();
    bool get(Side side, int x, int y);

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move m, Side side);
    MoveResult doMove(Move m, Side side);
    void undoMove(MoveResult m, Side side);
    vector<Move> getMoves(Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void display();

    void setBoard(char data[]);

    BoardNormalForm to_normal_form();
};

#endif
