#include "logistello.hpp"

using namespace asimov;

BookLogistello::BookLogistello() {
    Board* b = new Board();
    Side toMove = BLACK;
    std::ifstream file(FILEPATH);
    std::string line;
    Move mv = Move(-1, -1);

    // Each line is an opening, each set of three characters is a move
    while (std::getline(file, line)) {
        line = line.substr(0, line.find_first_of(':'));

        while (line.length() >= 3) {
            mv = stringToMove(line.substr(0, 3));
            table.insert(std::make_pair(b->to_normal_form(), mv));
            b->doMove(mv, toMove);

            BoardNormalForm bn = b->to_normal_form();

            toMove = (toMove == BLACK) ? WHITE : BLACK;
            line = line.substr(3, line.length());
        }

        toMove = BLACK;
        *b = b->empty();
    }

    delete b;
}

BookLogistello::~BookLogistello() {}

Move BookLogistello::nextMove(Board* b) {
    BoardAnnotatedNormalForm bAnno = b->to_annotated_normal_form();
    BoardNormalForm bNorm = bAnno.normalForm;

    // See if we're still in the book
    if (table.find(bNorm) == table.end()) {
        inBook = false;
        return Move(-1, -1);
    }

    Move moveNormal = table.at(bNorm); // Currently, best move on a normalized board
    return bAnno.undoTransformation(moveNormal);
}

/**
 * @brief Turns a string of the form `+a1` used by Logistello into a move.
 */
Move BookLogistello::stringToMove(string s) {
    return Move(s.at(1) - 'a', s.at(2) - (CHARSHIFT + 1));
}
