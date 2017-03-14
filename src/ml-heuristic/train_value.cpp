#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include "load_wthor.hpp"

using namespace std;

int main(int argc, char const *argv[]) {

    if (argc != 2) {
        return 1;
    }

    int len = 0;
    WTHORGame * datas = load_wthor_games(argv[1], &len);

    for (size_t i = 0; i < len; i++) {

        asimov::BitBoard b;
        int n = 12;
        Side s = BLACK;
        WTHORGame gm = datas[i];
        do {
            b = get_bitboard_from_wthor(gm, n, &s);
            printf("%s %d\n", b.to_string(s), gm.rs);
            return;//TODO fix
            n++;
        } while (n < 40);
    }

    return 0;
}
