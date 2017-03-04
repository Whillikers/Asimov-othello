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
    Move(int x, int y) {
        this->x = x;
        this->y = y;
    }
    ~Move() {}

    int getX() { return x; }
    int getY() { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
};

#endif
