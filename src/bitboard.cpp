#include "bitboard.hpp"
#include <iostream>
#include <bitset>

using namespace asimov;

/*=============================================================================
 *=============================================================================
 * Bitwise operations
 *=============================================================================
 *=============================================================================
 */
bool BitBoard::onBoard(int x, int y) {
    return !(x < 0 || x >= 8 || y < 0 || y >= 8);
}


bool BitBoard::occupied(int x, int y) {
    return (black.rows[y] | white.rows[y]) & (1<<x);
}


bool BitBoard::get(Side s, int x, int y) {
    bool msk = (s==BLACK);
    return (
        ((black.rows[y] & (1<<x)) > 0 && msk) |
        ((white.rows[y] & (1<<x)) > 0 && !msk)
    );
}


void BitBoard::set(Side s, int x, int y) {
    unset(x,y);
    bool msk = (s==BLACK);
    black.rows[y] |= ((1 & msk) << x);
    white.rows[y] |= ((1 & !msk) << x);
}


void BitBoard::flip(int x, int y) {
    black.rows[y] ^= (1 << x);
    white.rows[y] ^= (1 << x);
}


void BitBoard::unset(int x, int y) {
    black.rows[y] &= ~(1 << x);
    white.rows[y] &= ~(1 << x);
}

/*=============================================================================
 *=============================================================================
 * Occluded Fill operations
 *
 * gen: sources of fill
 * pro: propagators of fill
 *=============================================================================
 *=============================================================================
 */

//bitmasks to filter out certain Files
const u64 notAFile = 0xfefefefefefefefe; // ~0x0101010101010101
const u64 notHFile = 0x7f7f7f7f7f7f7f7f; // ~0x8080808080808080

u64 soutOccl(u64 gen, u64 pro) {
    gen |= pro & (gen >>  8);
    pro &=       (pro >>  8);
    gen |= pro & (gen >> 16);
    pro &=       (pro >> 16);
    gen |= pro & (gen >> 32);
    return gen;
}

u64 nortOccl(u64 gen, u64 pro) {
    gen |= pro & (gen <<  8);
    pro &=       (pro <<  8);
    gen |= pro & (gen << 16);
    pro &=       (pro << 16);
    gen |= pro & (gen << 32);
    return gen;
}

u64 eastOccl(u64 gen, u64 pro) {
    pro &= notAFile;
    gen |= pro & (gen << 1);
    pro &=       (pro << 1);
    gen |= pro & (gen << 2);
    pro &=       (pro << 2);
    gen |= pro & (gen << 4);
    return gen;
}

u64 noEaOccl(u64 gen, u64 pro) {
    pro &= notAFile;
    gen |= pro & (gen <<  9);
    pro &=       (pro <<  9);
    gen |= pro & (gen << 18);
    pro &=       (pro << 18);
    gen |= pro & (gen << 36);
    return gen;
}

u64 soEaOccl(u64 gen, u64 pro) {
    pro &= notAFile;
    gen |= pro & (gen >>  7);
    pro &=       (pro >>  7);
    gen |= pro & (gen >> 14);
    pro &=       (pro >> 14);
    gen |= pro & (gen >> 28);
    return gen;
}

u64 westOccl(u64 gen, u64 pro) {
    pro &= notHFile;
    gen |= pro & (gen >> 1);
    pro &=       (pro >> 1);
    gen |= pro & (gen >> 2);
    pro &=       (pro >> 2);
    gen |= pro & (gen >> 4);
    return gen;
}

u64 soWeOccl(u64 gen, u64 pro) {
    pro &= notHFile;
    gen |= pro & (gen >>  9);
    pro &=       (pro >>  9);
    gen |= pro & (gen >> 18);
    pro &=       (pro >> 18);
    gen |= pro & (gen >> 36);
    return gen;
}

u64 noWeOccl(u64 gen, u64 pro) {
    pro &= notHFile;
    gen |= pro & (gen <<  7);
    pro &=       (pro <<  7);
    gen |= pro & (gen << 14);
    pro &=       (pro << 14);
    gen |= pro & (gen << 28);
    return gen;
}


u64 BitBoard::find_moves(u64 gen, u64 pro) {
    u64 moves = 0;
    u64 empty = ~(gen | pro);
    u64 tmp;

    tmp = soutOccl(gen, pro);
    tmp &= pro;
    moves |= (tmp >> 8) & empty;

    tmp = nortOccl(gen, pro);
    tmp &= pro;
    moves |= (tmp << 8) & empty;

    tmp = eastOccl(gen, pro);
    tmp &= pro;
    moves |= (tmp << 1) & notAFile & empty;

    tmp = westOccl(gen, pro);
    tmp &= pro;
    moves |= (tmp >> 1) & notHFile & empty;

    tmp = soEaOccl(gen, pro);
    tmp &= pro;
    moves |= (tmp >> 7) & notAFile & empty;

    tmp = soWeOccl(gen, pro);
    tmp &= pro;
    moves |= (tmp >> 9) & notHFile & empty;

    tmp = noEaOccl(gen, pro);
    tmp &= pro;
    moves |= (tmp << 9) & notAFile & empty;

    tmp = noWeOccl(gen, pro);
    tmp &= pro;
    moves |= (tmp << 7) & notHFile & empty;

    return moves;
}

/*=============================================================================
 *=============================================================================
 * Board operations
 *=============================================================================
 *=============================================================================
 */
BitBoard::BitBoard() {
    black.bitmask = 0;
    white.bitmask = 0;
    bmoves.bitmask = 0;
    wmoves.bitmask = 0;
    set(BLACK, 4, 3);
    set(BLACK, 3, 4);
    set(WHITE, 3, 3);
    set(WHITE, 4, 4);
    find_moves(BLACK);
}

bool BitBoard::is_done() {
    find_moves(BLACK);
    find_moves(WHITE);
    return bmoves.bitmask == 0 && wmoves.bitmask == 0;
}

void BitBoard::find_moves(Side s) {
    switch (s) {
    case BLACK:
        bmoves.bitmask = find_moves(black.bitmask, white.bitmask);
        break;
    case WHITE:
        wmoves.bitmask = find_moves(white.bitmask, black.bitmask);
        break;
    }
}


bool BitBoard::has_moves(Side s) {
    bool msk = (s == BLACK);
    return (
        (bmoves.bitmask > 0 && msk) ||
        (wmoves.bitmask > 0 && !msk)
    );
}


bool BitBoard::check_move(Move m, Side s) {
    //return true;
    bool msk = (s==BLACK);
    int x = m.getX();
    int y = m.getY();
    return (
        ((bmoves.rows[y] & (1<<x)) > 0 && msk) ||
        ((wmoves.rows[y] & (1<<x)) > 0 && !msk)
    );
}


MoveResult BitBoard::do_move(Move m, Side s) {
    MoveResult mr = MoveResult();
    // A nullptr move means pass.
    if (m.isPass()) return mr;


    int X = mr.x = m.getX();
    int Y = mr.y = m.getY();
    // Ignore if move is invalid.
    if (!check_move(m, s)) return mr;

    // data o;
    // o.bitmask = 0;
    // o.rows[Y] |= (1<<X);
    // u64 org, gen, pro, tmp, msk = 0;
    // org = o.bitmask;
    // if (s == BLACK) {
    //     gen = black.bitmask;
    //     pro = white.bitmask;
    // } else {
    //     gen = white.bitmask;
    //     pro = black.bitmask;
    // }
    // msk |= (nortOccl(gen, pro) & soutOccl(org, pro));
    // msk |= (soutOccl(gen, pro) & nortOccl(org, pro));
    // msk |= (eastOccl(gen, pro) & westOccl(org, pro));
    // msk |= (westOccl(gen, pro) & eastOccl(org, pro));
    // msk |= (noEaOccl(gen, pro) & soWeOccl(org, pro));
    // msk |= (soWeOccl(gen, pro) & noEaOccl(org, pro));
    // msk |= (noWeOccl(gen, pro) & soEaOccl(org, pro));
    // msk |= (soEaOccl(gen, pro) & noWeOccl(org, pro));
    //
    // black.bitmask ^= msk;
    // white.bitmask ^= msk;
    //
    // bitset<64> x(black.bitmask), y(white.bitmask), mask(msk);
    // cerr << x << endl << y << endl << mask << endl;



    Side other = OTHER_SIDE(s);
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int c = 0;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(s, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    c++;
                    set(s, x, y);
                    x += dx;
                    y += dy;
                }
                mr.set(dx, dy, c);
            }
        }
    }
    set(s, X, Y);

    // mr.msk = msk;
    mr.bm = bmoves.bitmask;
    mr.wm = wmoves.bitmask;

    find_moves(BLACK);
    find_moves(WHITE);
    return mr;
}


void BitBoard::undo_move(MoveResult mr, Side s) {
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
    //
    // black.bitmask ^= mr.msk;
    // white.bitmask ^= mr.msk;

    unset(mr.x, mr.y);

    bmoves.bitmask = mr.bm;
    wmoves.bitmask = mr.wm;
}


vector<Move> BitBoard::get_moves(Side s) {
    vector<Move> mvs;

    for (size_t x = 0; x < 8; x++) {
        for (size_t y = 0; y < 8; y++) {
            Move m(x,y);
            if (check_move(m, s)) {
                mvs.push_back(m);
            }
        }
    }

    return mvs;
}


int BitBoard::count(Side s) {
    return (s==BLACK)?count_black():count_white();
}


int BitBoard::count_black() {
    u64 cntr = black.bitmask;
    int count = 0;
    for (size_t i = 0; i < 64; i++) {
        count += cntr & 1;
        cntr >>= 1;
    }
    return count;
}


int BitBoard::count_white() {
    u64 cntr = white.bitmask;
    int count = 0;
    for (size_t i = 0; i < 64; i++) {
        count += cntr & 1;
        cntr >>= 1;
    }
    return count;
}

BoardNormalForm BitBoard::to_normal_form() {
    //TODO: do rotations.
    return make_pair(black.bitmask, white.bitmask);
}

void BitBoard::display(Side s) {
    std::cout << " 1 2 3 4 5 6 7 8" << std::endl;
    for (int y = 0; y < 8; y++) {
        std::cout << (y+1);
        for (int x = 0; x < 8; x++) {
            if (get(WHITE, x, y)) {
                std::cout << "W ";
            } else if (get(BLACK, x, y)) {
                std::cout << "B ";
            } else if (check_move(Move(x,y), s)) {
                std::cout << "* ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }

    if (s == BLACK) {
        for (int i = 0; i < 8; i++) {
            bitset<8> x (bmoves.rows[i]);
            cout << x << endl;
        }
    } else {
        for (int i = 0; i < 8; i++) {
            bitset<8> x (wmoves.rows[i]);
            cout << x << endl;
        }
    }
}
