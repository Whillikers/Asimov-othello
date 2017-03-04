#include "search_alg/minimax.hpp"

using namespace asimov;

/**
 * @brief Searches for the best move using the minimax algorithm.
 *
 * @param b The board to start the search at.
 * @param max_time The maximum time in ms to search for (unused)
 * @param max_depth The maximum game tree depth to search to.
 * @param turn Which player is currently moving.
 *
 * @returns The best move found in the search time
 */
virtual Move search(Board *b, int max_time, int max_depth, Side turn) {
    int b;
    float g;

    vector<Move> mvs = bd->getMoves(turn);

    if (turn == WHITE) {
        g = -std::numeric_limits<float>::infinity();
        for (int i = 0; i < mvs.size(); i++) {
            Board* nb = bd->copy();
            Move m = mvs[i];
            nb->doMove(&(mvs[i]), turn);

            float v = minimax(nb, max_depth-1, OTHER_SIDE(turn));
            if (v > g) {
                g = v;
                b = i;
            }

            delete nb;
        }
    } else {
        g = std::numeric_limits<float>::infinity();
        for (int i = 0; i < mvs.size(); i++) {
            Board* nb = bd->copy();
            nb->doMove(&(mvs[i]), turn);

            float v = minimax(nb, max_depth-1, OTHER_SIDE(turn));
            if (v < g) {
                g = v;
                b = i;
            }

            delete nb;
        }
    }

    return mvs[b];
}

/**
 * @brief Internal function used to search the game tree
 *
 * @param b The board to start the search at.
 * @param d The how many more layers to search.
 * @param turn Which player is currently moving.
 *
 * @returns The best minimax score of of the given board on the given turn.
 */
float minimax(Board *b, int d, Side turn) {
    if (d == 0) {
        return h->evaluate(bd);
    }

    float g;

    vector<Move> mvs = bd->getMoves(turn);

    if (turn == WHITE) {
        g = -std::numeric_limits<float>::infinity();
        for (int i = 0; i < mvs.size(); i++) {
            Board* nb = bd->copy();
            Move m = mvs[i];
            nb->doMove(&(mvs[i]), turn);

            g = max(g, this->alpha_beta_search(nb, d-1, OTHER_SIDE(turn)));

            delete nb;
        }
    } else {
        g = std::numeric_limits<float>::infinity();
        for (int i = 0; i < mvs.size(); i++) {
            Board* nb = bd->copy();
            nb->doMove(&(mvs[i]), turn);

            g = min(g, this->alpha_beta_search(nb, d-1, OTHER_SIDE(turn)));

            delete nb;
        }
    }

    return g;

}
