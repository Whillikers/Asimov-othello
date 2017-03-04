#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <vector>
#include <utility>
#include "common.hpp"
using namespace std;

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);

public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    vector<Move> getMoves(Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
    pair<unsigned long long, unsigned long long> to_normal_form();
};

#endif
