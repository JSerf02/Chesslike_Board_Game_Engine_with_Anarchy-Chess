#ifndef KING_H
#define KING_H

#include "ChessPiece.h"

namespace chess {
    using namespace logic;

    class King : public ChessPiece
    {
        /*
         * TODO: Define King class
        */
        public:
            using ChessPiece::ChessPiece;
            bool isKing() override { return true; }
    };

}
#endif