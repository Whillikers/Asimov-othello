#pragma once

#include "board.hpp"

namespace asimov {

    /**
     * @brief An abstract class that represents a look-up table of openings.
     *
     * The constructor should load the book from a text file and the destructor
     * should unload it.
     */
    class OpeningBook {
    private:

    public:
        virtual ~OpeningBook();
        virtual Move nextMove(Board* b); // Returns (-1, -1) if none is found
        bool inBook = true; // Are we still getting moves from the book?
    };
}
