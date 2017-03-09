#include "monte.hpp"
#include <cmath>
#include <ctime>

using namespace asimov;

Move SearchMonteCarlo::search(Board *b, int max_time, int max_depth, Side turn) {

    vector<Move> mvs = b->getMoves(turn);
    vector<int> ns(mvs.size());
    vector<int> wins(mvs.size());
    for (size_t i = 0; i < mvs.size(); i++) {
        ns[i] = 0;
        wins[i] = 0;
    }

    //int iter = 50000;

    cerr << "Searching..." << ((float)max_time)/1000.0 << endl;

    time_t start = time(nullptr);
    int moves_left = 64 - b->countWhite() - b->countBlack();

    size_t n = 1;

    for (; difftime(time(nullptr), start) < 0.1*(float)max_time/1000.0; n++) {
        float c = sqrt(2 * log(n));
        //choose highest
        int maxi = 0;
        float maxucb = c/sqrt(ns[0]);
        for (size_t i = 1; i < mvs.size(); i++) {
            float ucb = c/sqrt(ns[i]);
            if (ucb > maxucb) {
                maxucb = ucb;
                maxi = i;
            }
        }

        MoveResult r = b->doMove(mvs[maxi], turn);
        int score = simulate(b, OTHER_SIDE(turn));
        b->undoMove(r, turn);
        wins[maxi] += score * TURN_MAX(turn);
        ns[maxi] += 1;
    }

    float c = sqrt(2 * log(n));

    int maxi = 0;
    float maxucb = wins[0]/ns[0] + c/sqrt(ns[0]);
    for (size_t i = 1; i < mvs.size(); i++) {
        float ucb = wins[i]/ns[i] + c/sqrt(ns[i]);
        if (ucb > maxucb) {
            maxucb = ucb;
            maxi = i;
        }
    }

    return mvs[maxi];
}

int SearchMonteCarlo::simulate(Board *b, Side turn) {

    if (b->isDone()) {
        int cw = b->countWhite();
        int cb = b->countBlack();
        return cw-cb;
    }

    vector<Move> mvs = b->getMoves(turn);
    if (mvs.size() == 0) {
        return simulate(b, OTHER_SIDE(turn));
    } else {
        int ri = abs(rand())%mvs.size();

        MoveResult r = b->doMove(mvs[ri], turn);
        int score = simulate(b, OTHER_SIDE(turn));
        b->undoMove(r, turn);
        return score;
    }
}
