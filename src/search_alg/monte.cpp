#include "monte.hpp"
#include <cmath>
#include <ctime>
#include <iostream>

using namespace std;

using namespace asimov;

Move SearchMonteCarlo::search(BitBoard &b, int max_time, int max_depth, Side turn) {

    vector<Move> mvs = b.get_moves(turn);

    if (mvs.size() == 0) {
        cerr << "Passing!" << endl;
        return Move();
    }

    vector<int> ns(mvs.size());
    vector<int> wins(mvs.size());
    cerr << endl << "Moves:";
    for (size_t i = 0; i < mvs.size(); i++) {
        cerr << " (" << mvs[i].getX() << "," << mvs[i].getY() << ")";
        ns[i] = 0;
        wins[i] = 0;
    }
    cerr << endl;

    //int iter = 50000;

    cerr << "Searching..." << ((float)max_time)/1000.0 << endl;

    time_t start = time(nullptr);
    int moves_left = 64 - b.count_white() - b.count_black();

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

        MoveResult r = b.do_move(mvs[maxi], turn);
        int score = simulate(b, OTHER_SIDE(turn));
        b.undo_move(r, turn);
        wins[maxi] += score * TURN_MAX(turn);
        ns[maxi] += 1;
    }

    float c = sqrt(2 * log(n));

    int maxi = 0;
    float maxucb = wins[0]/ns[0] + c/sqrt(ns[0]);
    for (size_t i = 1; i < mvs.size(); i++) {
        float ucb = ((float)wins[i])/((float)ns[i]) + c/sqrt(ns[i]);
        if (ucb > maxucb) {
            maxucb = ucb;
            maxi = i;
        }
    }

    Move best = mvs[maxi];

    cerr << "Searched " << n << " games." << endl;
    cerr << "Best Move (" << best.getX() << "," << best.getY()
        << ") has expected score of " << ((float)wins[maxi])/((float)ns[maxi]) << endl;

    return mvs[maxi];
}

int SearchMonteCarlo::simulate(BitBoard &b, Side turn) {

    if (b.is_done()) {
        int cw = b.count_white();
        int cb = b.count_black();
        return cw-cb;
    }

    vector<Move> mvs = b.get_moves(turn);
    if (mvs.size() == 0) {
        return simulate(b, OTHER_SIDE(turn));
    } else {
        int ri = abs(rand())%mvs.size();

        MoveResult r = b.do_move(mvs[ri], turn);
        int score = simulate(b, OTHER_SIDE(turn));
        b.undo_move(r, turn);
        return score;
    }
}
