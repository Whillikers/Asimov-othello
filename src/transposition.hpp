#pragma once

#include "board.hpp"
#include <queue>
#include <map>
#include <utility>
#include <limits>

using namespace std;

namespace asimov {

    typedef pair<unsigned long long, unsigned long long> TranspositionKey;

    struct TableValue {
        float lowerbound;
        float upperbound;

        TableValue() {
            lowerbound = 0.0;
            upperbound = 0.0;
        }

        TableValue(const TableValue &other) {
            lowerbound = other.lowerbound;
            upperbound = other.upperbound;
        }

        TableValue(float low, float high) {
            lowerbound = low;
            upperbound = high;
        }
    };

    class TranspositionTable {
    private:
        int max_size;
        unsigned long long current;
        //priority_queue<pair<unsigned long long, TranspositionKey> > ages;
        map<TranspositionKey, TableValue> entries;
    public:
        TranspositionTable(int max);

        void store(Board *b, TableValue value);
        bool fetch(Board * b, TableValue * value);
        void clear();
    };
}