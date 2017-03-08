#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <vector>
#include <utility>
#include <iostream>
#include "common.hpp"
using namespace std;

typedef pair<unsigned long long, unsigned long long> BoardNormalForm;

struct BoardAnnotatedNormalForm {
    BoardNormalForm normalForm;
    int transformationIndex = 0; // Specifies the transformations to get this

    Move undoTransformation(Move m) { // Applies the transformations backwards
        switch (transformationIndex) {
            case 0: return m;
            case 1: return Move(m.getY(), 7 - m.getX());
            case 2: return Move(7 - m.getX(), 7 - m.getY());
            case 3: return Move(7 - m.getY(), m.getX());
            case 4: return Move(7 - m.getY(), 7 - m.getX());
            case 5: return Move(m.getX(), 7 - m.getY());
            case 6: return Move(m.getY(), m.getX());
            case 7: return Move(7 - m.getX(), m.getY());
            default: return Move(-1, -1);
};

struct MoveResult {
    int x, y, flipped[9];

    MoveResult() {
        for (int i = 0; i < 9; i++) {
            flipped[i] = 0;
>>>>>>> Implemented move transformations on annotated normal boards
        }
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
    BoardAnnotatedNormalForm to_annotated_normal_form();
};

#endif
