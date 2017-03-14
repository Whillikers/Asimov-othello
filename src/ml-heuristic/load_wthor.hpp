#pragma once
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "../bitboard.hpp"

#define RS_OFFSET   (6)
#define TS_OFFSET   (7)
#define MV_OFFSET   (8)
#define MV_LENGTH   (60)

using namespace std;

struct WTHORGame {
    int rs;//real score (# black pieces)
    int ts;//theoretical score (# black pieces)
    char moves[60];
};

asimov::BitBoard get_bitboard_from_wthor(WTHORGame &wthor, int n, Side * s, int * p) {
    asimov::BitBoard b;
    Side t = BLACK;

    int i = 0, c = 0;
    Move m;
    while (!b.is_done() && c < n) {
        //skip if the current player must pass
        if (!b.has_moves(t)) {
            t = OTHER_SIDE(t);
            c++;
            continue;
        }

        //get the move info
        m.x = (((int)wthor.moves[i])%10)-1;
        m.y = (((int)wthor.moves[i])/10)-1;

        //make sure the move is valid
        if (b.check_move(m, t)) {
            b.do_move(m, t);
            t = OTHER_SIDE(t);
            c++;
        }
        i++;
    }

    if (s != nullptr) {
        *s = OTHER_SIDE(t);
    }

    if (p != nullptr) {
        *p = c;
    }

    return b;
}

WTHORGame * load_wthor_games(const char * filename, int *nn) {
    ifstream df(filename);

    df.seekg(0, df.end);
    size_t len = df.tellg();

    df.seekg(16, df.beg);

    char buffer[68];

    WTHORGame * r = new WTHORGame[len/68];
    int n = 0;
    for (int i = 16; i < len; i+= 68) {
        df.read(buffer, 68);
        WTHORGame gm;
        gm.rs = ((signed char)buffer[RS_OFFSET]);
        gm.ts = ((signed char)buffer[TS_OFFSET]);
        memcpy(gm.moves, buffer + MV_OFFSET, MV_LENGTH);

        asimov::BitBoard b = get_bitboard_from_wthor(gm, 60, nullptr);

        gm.rs = b.count_black()-b.count_white();

        r[n] = gm;
        n++;
    }
    *nn = n;

    return r;
}
