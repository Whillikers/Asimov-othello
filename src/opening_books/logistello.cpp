#include "logistello.hpp"

using namespace asimov;

BookLogistello::BookLogistello() {

}

BookLogistello::~BookLogistello() {

}

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
