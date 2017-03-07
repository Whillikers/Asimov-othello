#ifndef __COMMON_H__
#define __COMMON_H__

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

#endif
