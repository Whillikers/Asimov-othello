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

usigned int popcount_64(u64) {
    x = (x & 0x5555555555555555ULL) + ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x & 0x0F0F0F0F0F0F0F0FULL) + ((x >> 4) & 0x0F0F0F0F0F0F0F0FULL);
    return (x * 0x0101010101010101ULL) >> 56;
}

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

u64 sout_one(u64 gen) {
    return gen >> 8;
}

u64 nortOccl(u64 gen, u64 pro) {
    gen |= pro & (gen <<  8);
    pro &=       (pro <<  8);
    gen |= pro & (gen << 16);
    pro &=       (pro << 16);
    gen |= pro & (gen << 32);
    return gen;
}

u64 nort_one(u64 gen) {
    return gen << 8;
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

u64 east_one(u64 gen) {
    gen &= notAFile;
    return gen << 1;
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

u64 noea_one(u64 gen) {
    gen &= notAFile;
    return gen << 9;
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

u64 soea_one(u64 gen) {
    gen &= notAFile;
    return gen >> 7;
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

u64 west_one(u64 gen) {
    gen &= notHFile;
    return gen >> 1;
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

u64 sowe_one(u64 gen) {
    gen &= notHFile;
    return gen >> 9;
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

u64 nowe_one(u64 gen) {
    gen &= notHFile;
    return gen << 7;
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

u64 flip_vertical(u64 x) {
   const u64 k1 = 0x00FF00FF00FF00FF;
   const u64 k2 = 0x0000FFFF0000FFFF;
   x = ((x >>  8) & k1) | ((x & k1) <<  8);
   x = ((x >> 16) & k2) | ((x & k2) << 16);
   x = ( x >> 32)       | ( x       << 32);
   return x;
}

u64 flip_horizontal (u64 x) {
   const u64 k1 = 0x5555555555555555;
   const u64 k2 = 0x3333333333333333;
   const u64 k4 = 0x0f0f0f0f0f0f0f0f;
   x = ((x >> 1) & k1) | ((x & k1) << 1);
   x = ((x >> 2) & k2) | ((x & k2) << 2);
   x = ((x >> 4) & k4) | ((x & k4) << 4);
   return x;
}

u64 flip_diag(u64 x) {
   u64 t;
   const u64 k1 = 0x5500550055005500;
   const u64 k2 = 0x3333000033330000;
   const u64 k4 = 0x0f0f0f0f00000000;
   t  = k4 & (x ^ (x << 28));
   x ^=       t ^ (t >> 28) ;
   t  = k2 & (x ^ (x << 14));
   x ^=       t ^ (t >> 14) ;
   t  = k1 & (x ^ (x <<  7));
   x ^=       t ^ (t >>  7) ;
   return x;
}

u64 rotate_90_ccw(u64 x) {
    return flip_diag(flip_vertical(x));
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
    nmoves = 0;
    set(BLACK, 4, 3);
    set(BLACK, 3, 4);
    set(WHITE, 3, 3);
    set(WHITE, 4, 4);
    find_moves(BLACK);
}

bool BitBoard::is_done() {
    // find_moves(BLACK);
    // find_moves(WHITE);
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
    return (
        ((bmoves.rows[m.y] & ((1&msk)<<(m.x))) != 0) ||
        ((wmoves.rows[m.y] & ((1&!msk)<<(m.x))) != 0)
    );
}


MoveResult BitBoard::do_move(Move m, Side s) {
    MoveResult mr = MoveResult();
    // A nullptr move means pass.
    if (m.isPass()) return mr;


    int X = mr.x = m.x;
    int Y = mr.y = m.y;
    // Ignore if move is invalid.
    //if (!check_move(m, s)) return mr;


    // Side other = OTHER_SIDE(s);
    // for (int dx = -1; dx <= 1; dx++) {
    //     for (int dy = -1; dy <= 1; dy++) {
    //         if (dy == 0 && dx == 0) continue;
    //
    //         int c = 0;
    //
    //         int x = X;
    //         int y = Y;
    //         do {
    //             x += dx;
    //             y += dy;
    //         } while (onBoard(x, y) && get(other, x, y));
    //
    //         if (onBoard(x, y) && get(s, x, y)) {
    //             x = X;
    //             y = Y;
    //             x += dx;
    //             y += dy;
    //             while (onBoard(x, y) && get(other, x, y)) {
    //                 c++;
    //                 set(s, x, y);
    //                 x += dx;
    //                 y += dy;
    //             }
    //             mr.set(dx, dy, c);
    //         }
    //     }
    // }

    u64 pro, gen, org, msk = 0;
    data o;
    o.bitmask = 0;
    o.rows[Y] |= (1<<X);
    org = o.bitmask;

    if (s == BLACK) {
        gen = black.bitmask;
        pro = white.bitmask;
    } else {
        gen = white.bitmask;
        pro = black.bitmask;
    }

    msk |= soutOccl(gen, pro) & nortOccl(org, pro);
    msk |= nortOccl(gen, pro) & soutOccl(org, pro);
    msk |= eastOccl(gen, pro) & westOccl(org, pro);
    msk |= westOccl(gen, pro) & eastOccl(org, pro);
    msk |= soWeOccl(gen, pro) & noEaOccl(org, pro);
    msk |= noEaOccl(gen, pro) & soWeOccl(org, pro);
    msk |= soEaOccl(gen, pro) & noWeOccl(org, pro);
    msk |= noWeOccl(gen, pro) & soEaOccl(org, pro);

    black.bitmask ^= msk;
    white.bitmask ^= msk;

    set(s, X, Y);
    find_moves(BLACK);
    find_moves(WHITE);
    nmoves++;
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

    unset(mr.x, mr.y);

    find_moves(BLACK);
    find_moves(WHITE);
    nmoves--;
}


void BitBoard::get_moves(Side s, Move * arr, int * n) {
    int i = 0;
    Move m;
    u64 msk = (s==BLACK)?bmoves.bitmask:wmoves.bitmask;
    for (size_t y = 0; y < 8; y++) {
        if (msk&1){
            arr[i].x = 0;
            arr[i].y = y;
            i++;
        }

        if (msk&2){
            arr[i].x = 1;
            arr[i].y = y;
            i++;
        }

        if (msk&4){
            arr[i].x = 2;
            arr[i].y = y;
            i++;
        }

        if (msk&8){
            arr[i].x = 3;
            arr[i].y = y;
            i++;
        }

        if (msk&16){
            arr[i].x = 4;
            arr[i].y = y;
            i++;
        }

        if (msk&32){
            arr[i].x = 5;
            arr[i].y = y;
            i++;
        }

        if (msk&64){
            arr[i].x = 6;
            arr[i].y = y;
            i++;
        }

        if (msk&128){
            arr[i].x = 7;
            arr[i].y = y;
            i++;
        }

        msk >>= 8;
    }
    *n = i;
}


int BitBoard::count(Side s) {
    return (s==BLACK)?count_black():count_white();
}


int BitBoard::count_black() {
    // u64 cntr = black.bitmask;
    // int count = 0;
    // for (size_t i = 0; i < 64; i++) {
    //     count += cntr & 1;
    //     cntr >>= 1;
    // }
    // return count;
    return popcount_64(black.bitmask);
}


int BitBoard::count_white() {
    // u64 cntr = white.bitmask;
    // int count = 0;
    // for (size_t i = 0; i < 64; i++) {
    //     count += cntr & 1;
    //     cntr >>= 1;
    // }
    // return count;

    return popcount_64(white.bitmask);
}

int BitBoard::count_moves(Side s) {
    // int count = 0;
    // u64 cntr = (s == BLACK)?bmoves.bitmask:wmoves.bitmask;
    // for (size_t i = 0; i < 64; i++) {
    //     count += cntr & 1;
    //     cntr >>= 1;
    // }
    // return count;
    return popcount_64((s == BLACK)?bmoves.bitmask:wmoves.bitmask);
}


u64 BitBoard::pieces(Side s) {
    return (s==BLACK)?black.bitmask:white.bitmask;
}

u64 BitBoard::stability(Side s) {
    //sides
    const u64   top = 255ULL,
                bot = 18374686479671623680ULL,
                lft = 72340172838076673ULL,
                rht = 9259542123273814144ULL;
    u64 gen = (s = BLACK)?black.bitmask:white.bitmask;
    u64 pcs = black.bitmask|white.bitmask;

    u64 vrt = nortOccl(bot & pcs, pcs) & soutOccl(top & pcs, pcs),
        hrz = eastOccl(lft & pcs, pcs) & westOccl(rht & pcs, pcs),
        dg1 = noEaOccl((bot|lft) & pcs, pcs) & soWeOccl((top|rht) & pcs, pcs),
        dg2 = noWeOccl((bot|rht) & pcs, pcs) & soEaOccl((top|lft) & pcs, pcs);

    u64 stb = (0x8100000000000081ULL | (vrt & hrz & dg1 & dg2)) & gen;

    //expand stable areas. At most 16 iterations necessary to reach from one
    //corner to the other
    for (size_t i = 0; i < 16; i++) {
        stb |= gen & (
            (nort_one(stb) | sout_one(stb) | vrt) &
            (east_one(stb) | west_one(stb) | hrz) &
            (noea_one(stb) | sowe_one(stb) | dg1) &
            (nowe_one(stb) | soea_one(stb) | dg2)
        );
    }

    return stb;
}

u64 BitBoard::mobility(Side s) {
    return (s==BLACK)?bmoves.bitmask:wmoves.bitmask;
}

BoardNormalForm BitBoard::to_normal_form() {
    u64 b = black.bitmask, w = white.bitmask;
    BoardNormalForm mn = make_pair(b,w);
    for (size_t i = 0; i < 3; i++) {
        b = rotate_90_ccw(b);
        w = rotate_90_ccw(w);
        mn = min(mn, make_pair(b,w));
    }
    b = flip_horizontal(b);
    mn = min(mn, make_pair(b,w));
    for (size_t i = 0; i < 3; i++) {
        b = rotate_90_ccw(b);
        w = rotate_90_ccw(w);
        mn = min(mn, make_pair(b,w));
    }
    return mn;
}

void BitBoard::display(Side s) {
    cout << endl << hash() << endl;
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

    // if (s == BLACK) {
    //     for (int i = 0; i < 8; i++) {
    //         bitset<8> x (bmoves.rows[i]);
    //         cout << x << endl;
    //     }
    // } else {
    //     for (int i = 0; i < 8; i++) {
    //         bitset<8> x (wmoves.rows[i]);
    //         cout << x << endl;
    //     }
    // }
}

//http://www.isthe.com/chongo/tech/comp/fnv/
u64 BitBoard::hash() {
    //64-bit hash prime
    const u64 fnv_prime = 1099511628211ULL;
    //offset basis
    u64 hash = 14695981039346656037ULL, b, w;
    b = black.bitmask;
    w = white.bitmask;
    for (int i = 0; i < 8; i++) {
        hash = (hash*fnv_prime)^(b & 0xFF);
        hash = (hash*fnv_prime)^(w & 0xFF);
        b >>= 8;
        w >>= 8;
    }
    return hash;
}

char * BitBoard::to_string(Side s) {
    char * buf = new char[129];
    buf[128] = 0;

    u64 b = black.bitmask;
    u64 w = white.bitmask;

    for (size_t i = 0; i < 64; i++) {
        buf[i] = (b & 1)?'1':'0';
        b >>= 1;
        buf[i+64] = (w & 1)?'1':'0';
        w >>= 1;
    }

    return buf;
}
