#include "alphabeta.hpp"

#include "mtdf.hpp"
#include "../asimov_common.hpp"

#include <vector>

#define SET_MAX(a, b, q, r)     {if (q > a) {a = q; b = r;}}
#define SET_MIN(a, b, q, r)     {if (q < a) {a = q; b = r;}}

using namespace std;
using namespace asimov;

SearchAlphaBeta::SearchAlphaBeta(Heuristic *_h) : Search(_h) {
    table = new TranspositionTable(MAX_TRANSPOSITION_SIZE);
}

SearchAlphaBeta::~SearchAlphaBeta() {
    table->clear();
    delete table;
}

/**
 * @brief Implementation of iterative deepening alpha-beta tree search algorithm.
 */
Move SearchAlphaBeta::search(BitBoard &b, int max_time, int max_depth, Side turn) {
    int d = 3;
    float sc, guess;
    time_t start;
    Move mbst(0,0);

    sc = TURN_MAX(turn);
    guess = 0.0;

    cerr << endl << "[" << ((float)max_time)/1000.0 << "] Searching..." << endl;

    time(&start);
    do {
        table->clear();

        float inf = std::numeric_limits<float>::infinity();

        ABResult r = alpha_beta_search(b, -inf, inf, d, turn);

        mbst = r.m;

        d++;
    } while (
        difftime(time(nullptr), start) < 0.02*((float)max_time)/1000.0 &&
        d < max_depth
    );

    cerr << "Searched to depth " << d << endl << endl;

    return mbst;
}

/**
 * @brief Alpha beta minimax search "with memory" (a.k.a with a transposition
 * table).
 */
ABResult SearchAlphaBeta::alpha_beta_search(BitBoard &bd, float a, float b, int d, Side turn) {

    //cerr << a << ":" << b << endl;
    //retrieve from transposition table
    TableValue entry;
    entry.lowerbound = -std::numeric_limits<float>::infinity();
    entry.upperbound = std::numeric_limits<float>::infinity();
    if (table != nullptr) {
        if (table->fetch(bd, &entry)) {
            if (entry.lowerbound >= b) {return ABResult(Move(), entry.lowerbound);}
            if (entry.upperbound <= a) {return ABResult(Move(), entry.upperbound);}
            a = max(a, entry.lowerbound);
            b = min(b, entry.upperbound);
        }
    }
    //cerr << a << ":" << b << endl;

    if (bd.is_done()) {
        return ABResult(Move(), 100.0*(bd.count_white()-bd.count_black()));
    }

    if (d == 0) {
        return ABResult(Move(), h->evaluate(bd));
    }

    float sc, g;
    int best = 0;

    sc = TURN_MAX(turn);

    vector<Move> mvs = bd.get_moves(turn);
    if (mvs.size() == 0) {
        //return a pass if we have no moves
        ABResult r = alpha_beta_search(bd, a, b, d-1, OTHER_SIDE(turn));
        return ABResult(Move(), r.score);
    }

    if (turn == WHITE) {
        g = -std::numeric_limits<float>::infinity();
        float alpha = a;
        for (int i = 0; i < mvs.size() && g < b; i++) {
            MoveResult res = bd.do_move(mvs[i], turn);

            ABResult v = alpha_beta_search(bd, alpha, b, d-1, OTHER_SIDE(turn));

            SET_MAX(g, best, v.score, i);
            alpha = max(alpha, g);

            bd.undo_move(res, turn);
        }
    } else {
        g = std::numeric_limits<float>::infinity();
        float beta = b;
        for (int i = 0; i < mvs.size() && a < g; i++) {
            MoveResult res = bd.do_move(mvs[i], turn);

            ABResult v = alpha_beta_search(bd, a, beta, d-1, OTHER_SIDE(turn));

            SET_MIN(g, best, v.score, i);
            beta = min(beta, g);

            bd.undo_move(res, turn);
        }
    }
    //cerr << "C\n";

    //update bounds
    if (g <= a) {entry.upperbound = g;}
    if (g >= b) {entry.lowerbound = g;}
    if (g > a && g < b) {
        entry.upperbound = g;
        entry.lowerbound = g;
    }

    if (table != nullptr) {
        table->store(bd, entry);
    }

    return ABResult(mvs[best], g);
}
