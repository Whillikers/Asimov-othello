#include "board.hpp"

Board Board::empty() {
    Board b;
    b.taken.reset();
    b.black.reset();

    return b;
}

/**
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

void Board::flip(int x, int y) {
    black.flip(x + 8*y);
}

void Board::clear(int x, int y) {
    taken.reset(x + 8*y);
    black.reset(x + 8*y);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move m, Side side) {
    // Passing is only legal if you have no moves.
    if (m.isPass()) return !hasMoves(side);

    int X = m.getX();
    int Y = m.getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/**
 * Modifies the board to reflect the specified move.
 */
MoveResult Board::doMove(Move m, Side side) {
    MoveResult mr = MoveResult();
    // A nullptr move means pass.
    if (m.isPass()) return mr;


    int X = mr.x = m.getX();
    int Y = mr.y = m.getY();
    // Ignore if move is invalid.
    if (!checkMove(m, side)) return mr;


    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -2; dx <= 0; dx++) {
        for (int dy = -2; dy <= 0; dy++) {
            if (dy == -1 && dx == -1) continue;

            int c = -1;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    c++;
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
                mr.set(dx, dy, c);
            }
        }
    }
    set(side, X, Y);
    return mr;
}

/**
 * @brief Undoes the last move (described by `mr` is returned by doMove)
 */
void Board::undoMove(MoveResult mr, Side s) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;
            int c = mr.get(dx, dy);

            int x = mr.x;
            int y = mr.y;

            for(int i = 0; i < c; i++) {
                x += dx;
                y += dy;
                flip(x,y);
            }
        }
    }

    clear(mr.x, mr.y);
}

/**
 * Returns true if there are legal moves for the given side.
 */
vector<Move> Board::getMoves(Side side) {
    vector<Move> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(move, side)) {
                moves.push_back(move);
            }
        }
    }
    // TODO: Find the root cause of the bug this terrible hack fixes
    for (int i = 0; i < moves.size(); i++) {
        moves[i].getX();
    }
    return moves;
}

/**
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/**
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/**
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/**
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}

/**
 * @brief Rotates the board 90 degrees CCW
 */
Board Board::rotate_90_ccw() {
    Board b = Board();
    for (size_t x = 0; x < 8; x++) {
        for (size_t y = 0; y < 8; y++) {
            if (get(WHITE, x, y)) {
                b.set(WHITE, 7-y, x);
            } else if (get(BLACK, x, y)) {
                b.set(BLACK, 7-y, x);
            }
        }
    }
    return b;
}

/**
 * @brief Flips the board on the x axis
 */
Board Board::flip_horizontal() {
    Board b = Board();
    for (size_t x = 0; x < 8; x++) {
        for (size_t y = 0; y < 8; y++) {
            if (get(WHITE, x, y)) {
                b.set(WHITE, 7-x, y);
            } else if (get(BLACK, x, y)) {
                b.set(BLACK, 7-x, y);
            }
        }
    }
    return b;
}

BoardNormalForm Board::to_num_form() {
    return make_pair(taken.to_ullong(), black.to_ullong());
}

BoardNormalForm Board::to_normal_form() {
    Board curr = *this;
    BoardNormalForm tmp, minimum = to_num_form();
    for (size_t i = 0; i < 3; i++) {
        curr = curr.rotate_90_ccw();
        tmp = curr.to_num_form();
        if (tmp < minimum) {
            minimum = tmp;
        }
    }
    curr = curr.flip_horizontal();
    tmp = curr.to_num_form();
    if (tmp < minimum) {
        minimum = tmp;
    }
    for (size_t i = 0; i < 3; i++) {
        curr = curr.rotate_90_ccw();
        tmp = curr.to_num_form();
        if (tmp < minimum) {
            minimum = tmp;
        }
    }

    return minimum;
}

BoardAnnotatedNormalForm Board::to_annotated_normal_form() {
    Board curr = *this;
    BoardNormalForm tmp, minimum = to_num_form();
    BoardAnnotatedNormalForm annotated;
    int transformations = 0;

    for (size_t i = 0; i < 3; i++) {
        curr = curr.rotate_90_ccw();
        tmp = curr.to_num_form();
        if (tmp < minimum) {
            minimum = tmp;
            transformations = i + 1;
        }
    }
    curr = curr.flip_horizontal();
    tmp = curr.to_num_form();
    if (tmp < minimum) {
        minimum = tmp;
        transformations = 4;
    }
    for (size_t i = 0; i < 3; i++) {
        curr = curr.rotate_90_ccw();
        tmp = curr.to_num_form();
        if (tmp < minimum) {
            minimum = tmp;
            transformations = 5 + i;
        }
    }

    annotated.normalForm = minimum;
    annotated.transformationIndex = transformations;

    return annotated;
}

void Board::display() {
    std::cout << " 1 2 3 4 5 6 7 8" << std::endl;
    for (int y = 0; y < 8; y++) {
        std::cout << (y+1);
        for (int x = 0; x < 8; x++) {
            if (get(WHITE, x, y)) {
                std::cout << "W ";
            } else if (get(BLACK, x, y)) {
                std::cout << "B ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
}
