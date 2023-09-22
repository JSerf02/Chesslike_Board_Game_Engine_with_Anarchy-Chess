#ifndef BISHOP_H
#define BISHOP_H

#include "ChessPiece.h"

namespace chess {
    using namespace logic;

    /* A chess piece with a value of 3.0 that moves along diagonals */
    class Bishop : public ChessPiece
    {
        public:
            /* 
            * Inherit the ChessPiece constructors 
            */
            using ChessPiece::ChessPiece;

            /*
            * Returns the value of the piece, 3.0
            */
            double getValue() override;

        private:
            /*
            * Returns moves along diagonals that are not blocked
            */
            std::vector<Move> generateMoves(GameState& gameState) override;

            /*
            * Returns moves along diagonals that are not blocked
            */
            std::vector<Move> generateAttackingMoves(GameState& gameState) override;
    };

}
#endif