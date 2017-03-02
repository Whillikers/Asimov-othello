#pragma once

namespace asimov {
    enum HeuristicType {
        LINEAR_FIT
    };

    class Heuristic {
    private:

    public:
        Heuristic();
        float evaluate(Board *b, HeuristicType type);
    };
}
