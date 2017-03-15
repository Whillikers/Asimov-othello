#include "alphabeta.hpp"

#include "mtdf.hpp"
#include "../asimov_common.hpp"

#include <vector>
#include <algorithm>

#define SEARCH_DEPTH    (7)
#define MONTE_ITERS     (3)
#define INF             (std::numeric_limits<float>::infinity())

#define SET_MAX(a, b, q, r)     {if (q > a) {a = q; b = r;}}
#define SET_MIN(a, b, q, r)     {if (q < a) {a = q; b = r;}}

using namespace std;
using namespace asimov;

SearchAlphaBeta::SearchAlphaBeta(Heuristic *_h) : Search(_h) {
    table = new TranspositionTable(MAX_TRANSPOSITION_SIZE);
    low = -INF;
    high = INF;
}


SearchAlphaBeta::SearchAlphaBeta(Heuristic *_h, float l, float h) : Search(_h) {
    table = new TranspositionTable(MAX_TRANSPOSITION_SIZE);
    low = l;
    high = h;
}

SearchAlphaBeta::~SearchAlphaBeta() {
    table->clear();
    delete table;
}

//*
int simulate(BitBoard &b, Side turn) {
    int n, i = 0;
    Move mvs[MAX_MOVES];

    BitBoard bt = b;

    while (!bt.is_done()) {

        bt.get_moves(turn, mvs, &n);
        if (n != 0) {
            int ri = abs(rand())%n;

            bt.do_move(mvs[ri], turn);
        }
        turn = OTHER_SIDE(turn);
        i++;
    }
    int cw = bt.count_white();
    int cb = bt.count_black();
    return cb-cw;
}
// */

/*
float alpha_beta_simple(BitBoard &bd, float a, float b, int d, Side turn) {
    int n = 0;
    Move mvs[MAX_MOVES];

    //should not be hit unless we are using the solver
    if (bd.is_done()) {
        return h->evaluate(bd);
        //return bd.count_white()-bd.count_black();
    }

    if (d == 0) {
        return h->evaluate(bd);
    }

    float g;

    bd.get_moves(turn, mvs, &n);
    if (n == 0) {
        //pass if there are no moves
        return alpha_beta_simple(bd, a, b, d-1, OTHER_SIDE(turn));
    }

    if (turn == WHITE) {
        g = -INF;
        float alpha = a;
        for (int i = 0; i < n && g < b; i++) {

            BitBoard bt = bd;
            bt.do_move(mvs[i], turn);

            float v = alpha_beta_simple(bt, alpha, b, d-1, OTHER_SIDE(turn));

            g = max(g, v);
            alpha = max(alpha, g);
        }
    } else {
        g = INF;
        float beta = b;
        for (int i = 0; i < n && a < g; i++) {

            BitBoard bt = bd;
            bt.do_move(mvs[i], turn);

            float v = alpha_beta_simple(bt, a, beta, d-1, OTHER_SIDE(turn));

            g = min(g, v);
            beta = min(beta, g);
        }
    }

    return g;
}
// */

//functions to sort pairs by their first float value in order to order moves.
bool pair_less(pair<float, int> a, pair<float, int> b) {
    return a.first < b.first;
}
bool pair_more(pair<float, int> a, pair<float, int> b) {
    return a.first > b.first;
}


float SearchAlphaBeta::alpha_beta_search(BitBoard &bd, float a, float b, int d, Side turn) {
    int n = 0;
    Move rmvs[MAX_MOVES];
    pair<float, int> mvs[MAX_MOVES];

    //should not be hit unless we are using the endgame solver
    if (bd.is_done()) {
        return h->evaluate(bd, turn);
    }

    if (d == 0) {
    
        float total = 0.0;
        for (int i = 0; i < 3; i++) {
            total += simulate(bd, turn);
        }
        return total/4.0;
        //return h->evaluate(bd, turn);
    }

    float g;

    bd.get_moves(turn, rmvs, &n);
    if (n == 0) {
        //pass if there are no moves
        return alpha_beta_search(bd, a, b, d-1, OTHER_SIDE(turn));
    }
    
    for (int i = 0; i < n; i++) {
        mvs[i] = make_pair(rand()/RAND_MAX,i);
    }

    //seed moves and sort them
    h->order_moves(bd, turn, n, rmvs, mvs);


    if (turn == BLACK) {
        g = -INF;
        //sort the moves by their estimated score
        sort(mvs, mvs+(n+1), pair_more);
        float alpha = a;
        for (int i = 0; i < n && alpha < b; i++) {
            BitBoard btmp = bd;
            btmp.do_move(rmvs[mvs[i].second], turn);

            float v = alpha_beta_search(btmp, alpha, b, d-1, OTHER_SIDE(turn));

            g = max(g, v);
            alpha = max(alpha, g);
        }
    } else {
        g = INF;
        //sort the moves by their estimated score
        sort(mvs, mvs+(n+1), pair_less);
        float beta = b;
        for (int i = 0; i < n && a < beta; i++) {
            BitBoard btmp = bd;
            btmp.do_move(rmvs[mvs[i].second], turn);

            float v = alpha_beta_search(btmp, a, beta, d-1, OTHER_SIDE(turn));

            g = min(g, v);
            beta = min(beta, g);
        }
    }

    return g;
}


/**
 * @brief Implementation of iterative deepening alpha-beta tree search algorithm.
 */
Move SearchAlphaBeta::search(BitBoard &b, int max_time, int max_depth, Side turn) {
    // int d = 3;
    // float sc, guess;
    // time_t start;
    // Move mbst(0,0);
    //
    // sc = TURN_MAX(turn);
    // guess = 0.0;
    //
    // cerr << endl << "[" << ((float)max_time)/1000.0 << "] Searching..." << endl;
    //
    // time(&start);
    // do {
    //     table->clear();
    //
    //
    //
    //     ABResult r = alpha_beta_search(b, -inf, inf, d, turn);
    //
    //     mbst = r.m;
    //
    //     d++;
    // } while (
    //     difftime(time(nullptr), start) < 0.02*((float)max_time)/1000.0 &&
    //     d < max_depth
    // );
    //
    // cerr << "Searched to depth " << d << endl << endl;
    //
    // return mbst;
    bool guess_missed = true;
    int best = 0, n = 0;
    float g;
    Move mvs[MAX_MOVES];

    b.get_moves(turn, mvs, &n);
    if (n == 0) {
        //return pass if there are no moves
        return Move();
    }

    float alpha = low;
    float beta = high;
    
    cerr << endl << "Searcing..." << endl;
    
    do {
        if (turn == BLACK) {
            g = -INF;
            for (int i = 0; i < n; i++) {
                BitBoard btmp = b;
                btmp.do_move(mvs[i], turn);

                float v = alpha_beta_search(btmp, alpha, high, max_depth, OTHER_SIDE(turn));

                SET_MAX(g, best, v, i);
                alpha = max(alpha, g);
            }
        } else {
            g = INF;
            for (int i = 0; i < n; i++) {
                BitBoard btmp = b;
                btmp.do_move(mvs[i], turn);

                float v = alpha_beta_search(btmp, low, beta, max_depth, OTHER_SIDE(turn));

                SET_MIN(g, best, v, i);
                beta = min(beta, g);
            }
        }

        //if we fail to find a move, expand the search area
        alpha = -INF;
        beta = INF;
    } while (false);
    
    cerr << "Best move: (" << mvs[best].x << "," << mvs[best].y << ") score: " << g << endl << endl;
    
    return mvs[best];
}
