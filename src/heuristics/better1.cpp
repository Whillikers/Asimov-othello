#include "heuristics/better1.hpp"

using namespace asimov;

Better1Heuristic::Better1Heuristic() {

}

Better1Heuristic::~Better1Heuristic() {

}

/**
 * @brief Better1Heuristic is slightly more advanced.
 */
float Better1Heuristic::evaluate(BitBoard &b) {
    float score = 0.0;
    float corner = 11.0;
    float corner_access = 1.1;
    float edge = 6.0;
    float edge_access = 1.25;
    float other = 2.0;
    float mobility = 5.0;

    for (size_t x = 0; x < 8; x++) {
        for (size_t y = 0; y < 8; y++) {

            float sc = 0.0;
            if (b.get(WHITE,x,y)) {
                sc = 1.0;
            } else if (b.get(BLACK,x,y)) {
                sc = -1.0;
            }

            if ((x==0 || x == 7) && (y == 0 || y == 7)) {
                score += sc * corner;
            } else if ((x <= 1 || x >= 6) && (y <= 1 || y >= 6)) {
                score += sc * corner_access;
            } else if (x == 0 || x == 7 || y == 0 || y == 7) {
                score += sc * edge;
            } else if (x <= 1 || x >= 6 || y <= 1 || y >= 6) {
                score += sc * edge_access;
            } else {
                score += sc * other;
            }
        }
    }

    score += (b.count_moves(WHITE) - b.count_moves(BLACK)) * mobility;

    return score;
}
