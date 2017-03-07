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
Move SearchMinimax::search(Board *b, int max_time, int max_depth, Side turn) {
    int bst;
    float g;

    vector<Move> mvs = b->getMoves(turn);

    if (turn == WHITE) {
        g = -std::numeric_limits<float>::infinity();
        for (int i = 0; i < mvs.size(); i++) {
            MoveResult res = b->doMove(mvs[i], turn);

            float v = minimax(b, max_depth-1, OTHER_SIDE(turn));
            if (v > g) {
                g = v;
                bst = i;
            }

            b->undoMove(res, turn);
        }
    } else {
        g = std::numeric_limits<float>::infinity();
        for (int i = 0; i < mvs.size(); i++) {
            MoveResult res = b->doMove(mvs[i], turn);

            float v = minimax(b, max_depth-1, OTHER_SIDE(turn));
            if (v < g) {
                g = v;
                bst = i;
            }

            b->undoMove(res, turn);
        }
    }

    return mvs[bst];
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
float SearchMinimax::minimax(Board *b, int d, Side turn) {
    if (d == 0 || b->isDone()) {
        return h->evaluate(b);
    }

    float g;

    vector<Move> mvs = b->getMoves(turn);

    if (turn == WHITE) {
        g = -std::numeric_limits<float>::infinity();
        for (int i = 0; i < mvs.size(); i++) {
            MoveResult res = b->doMove(mvs[i], turn);

            g = max(g, minimax(b, d-1, OTHER_SIDE(turn)));

            b->undoMove(res, turn);
        }
    } else {
        g = std::numeric_limits<float>::infinity();
        for (int i = 0; i < mvs.size(); i++) {
            MoveResult res = b->doMove(mvs[i], turn);

            g = min(g, minimax(b, d-1, OTHER_SIDE(turn)));

            b->undoMove(res, turn);
        }
    }

    return g;

}
