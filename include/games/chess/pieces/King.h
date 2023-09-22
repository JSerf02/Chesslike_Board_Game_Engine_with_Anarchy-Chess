#ifndef KING_H
#define KING_H

#include "ChessPiece.h"

namespace chess {
    using namespace logic;

    /* A chess piece that moves to spaces a maximum of 1 tile away */
    class King : public ChessPiece
    {
        public:
            /* 
            * Inherit the ChessPiece constructors 
            */
            using ChessPiece::ChessPiece;

            /*
            * Returns the value of the piece, 100.0 
            * - 100.0 was chosen since it is a really big number, though kings
            *   should never be captured so the value shouldn't matter
            */
            double getValue() override;

            /*
            * Returns true since this is the king!
            */
            bool isKing() override;

        private:
            /*
            * Returns moves that are 1 distance away that are not blocked
            */
            std::vector<Move> generateMoves(GameState& gameState) override;

            /*
            * Returns moves that are 1 distance away that are not blocked
            */
            std::vector<Move> generateAttackingMoves(GameState& gameState) override;
    };

}
#endif