#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include "load_wthor.hpp"
#include "../common.hpp"

using namespace std;

int main(int argc, char const *argv[]) {

    if (argc != 2) {
        return 1;
    }

    int len = 0;
    WTHORGame * datas = load_wthor_games(argv[1], &len);

    for (size_t i = 0; i < len; i++) {

        asimov::BitBoard b;
        int n = 8;
        Side s = BLACK;
        WTHORGame gm = datas[i];
        do {
            int r = 0;
            b = get_bitboard_from_wthor(gm, n, &s, &r);
            char stab[129], smv[65];
            stab[128] = 0;
            smv[64] = 0;

            //get the stability of each players' pieces
            u64 stb_b = b.stability(BLACK);
            u64 stb_w = b.stability(WHITE);

            //get next move
            int mx, my;
            mx = (((int)gm.moves[r])%10) - 1;
            my = (((int)gm.moves[r])/10) - 1;

            u64 mv = ((1ULL) << (mx + (8*my)));

            for (int i = 0; i < 64; i++) {
                stab[i]    = (stb_b & 1)?'1':'0';
                stab[i+64] = (stb_w & 1)?'1':'0';
                stb_b >>= 1;
                stb_w >>= 1;
                smv[i] = (mv & 1)?'1':'0';
                mv >>=1;
            }

            printf("%s%s %s\n", b.to_string(s), stab, smv);
            n++;
        } while (n < 48);
    }

    return 0;
}
