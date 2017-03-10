#ifndef __COMMON_H__
#define __COMMON_H__

#include <utility>

enum Side {
    WHITE, BLACK
};

#define OTHER_SIDE(s)   (((s)==WHITE)?BLACK:WHITE)
#define TURN_MAX(s)     (((s)==WHITE)?1.0:-1.0)

class Move {

public:

    int x, y;

    static Move pass() {
        return Move(-1,-1);
    }
    Move(int x, int y) {
        this->x = x;
        this->y = y;
    }
    ~Move() {}

    bool isPass() {
        return x == -1 && y == -1;
    }

    int getX() { return x; }
    int getY() { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
};

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

typedef std::pair<unsigned long long, unsigned long long> BoardNormalForm;

#endif
