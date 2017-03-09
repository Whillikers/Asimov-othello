#include "mtdf.hpp"
#include "../asimov_common.hpp"

#include <vector>

using namespace std;
using namespace asimov;

SearchMTDf::SearchMTDf(Heuristic *_h) : Search(_h) {
    table = new TranspositionTable(MAX_TRANSPOSITION_SIZE);
}

SearchMTDf::~SearchMTDf() {
    table->clear();
    delete table;
}

/**
 * @brief Implementation of iterative deepening MTDf algorithm.
 */
Move SearchMTDf::search(Board *b, int max_time, int max_depth, Side turn) {
    int d = 1;
    float sc, guess;
    time_t start, current;
    Move mbst(0,0);

    sc = TURN_MAX(turn);
    guess = 0.0;

    cerr << "Depth:" << endl;

    current = time(&start);
    do {
        cerr << d << endl;

        pair<Move, float> r = this->mtdf(b, guess, d, turn);

        guess = r.second;
        mbst = r.first;

        time(&current);
        d++;
    } while (difftime(current, start) < max_time && d < max_depth);

    return mbst;
}

/**
 * @brief Implementation of fixed depth MTDf algorithm.
 */
pair<Move, float> SearchMTDf::mtdf(Board *b, float guess, int d, Side turn) {
    int mbst;
    float bst, low, high, beta, sc;


    low = bst = -(high = std::numeric_limits<float>::infinity());
    sc = TURN_MAX(turn);

    vector<Move> mvs = b->getMoves(turn);

    table->clear();

    while (low < high) {
        bst = -std::numeric_limits<float>::infinity();

        beta = guess;
        if (guess == low) {
            beta += 0.05;
        }

        for (int i = 0; i < mvs.size(); i++) {

            MoveResult res = b->doMove(mvs[i], turn);
            float v = sc * alpha_beta_search(b, beta-0.05, beta, d, OTHER_SIDE(turn));
            b->undoMove(res, turn);

            if (v > bst) {
                bst = v;
                mbst = i;
            }
        }

        guess = sc * bst;

        if (guess < beta) {
            high = guess;
        } else {
            low = guess;
        }
    }

    return make_pair(mvs[mbst], guess);
}

/**
 * @brief Alpha beta minimax search "with memory" (a.k.a with a transposition
 * table).
 */
float SearchMTDf::alpha_beta_search(Board *bd, float a, float b, int d, Side turn) {

    //cerr << a << ":" << b << endl;
    //retrieve from transposition table
    TableValue entry;
    entry.lowerbound = -std::numeric_limits<float>::infinity();
    entry.upperbound = std::numeric_limits<float>::infinity();
    if (table != nullptr) {
        if (table->fetch(bd, &entry)) {
            if (entry.lowerbound >= b) {return entry.lowerbound;}
            if (entry.upperbound <= a) {return entry.upperbound;}
            a = max(a, entry.lowerbound);
            b = min(b, entry.upperbound);
        }
    }
    //cerr << a << ":" << b << endl;

    if (d == 0 || bd->isDone()) {
        return this->h->evaluate(bd);
    }

    float sc, g;

    sc = TURN_MAX(turn);
    vector<Move> mvs = bd->getMoves(turn);

    if (mvs.size() == 0) {
        return alpha_beta_search(bd, a, b, d-1, OTHER_SIDE(turn));
    }

    if (turn == WHITE) {
        g = -std::numeric_limits<float>::infinity();
        float alpha = a;
        for (int i = 0; i < mvs.size() && g < b; i++) {
            MoveResult res = bd->doMove(mvs[i], turn);

            float v = alpha_beta_search(bd, alpha, b, d-1, OTHER_SIDE(turn));

            g = max(g, v);
            alpha = max(alpha, g);

            bd->undoMove(res, turn);
        }
    } else {
        g = std::numeric_limits<float>::infinity();
        float beta = b;
        for (int i = 0; i < mvs.size() && a < g; i++) {
            MoveResult res = bd->doMove(mvs[i], turn);

            float v = alpha_beta_search(bd, a, beta, d-1, OTHER_SIDE(turn));

            g = min(g, v);
            beta = min(beta, g);

            bd->undoMove(res, turn);
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

    return g;
}
