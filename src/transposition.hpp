#pragma once

namespace asimov {

    class TranspositionTable {
    private:
        int max_size;
    public:
        TranspositionTable(int max);

        bool fetch(Board * b, float * value);
        void clear();
    };
}
