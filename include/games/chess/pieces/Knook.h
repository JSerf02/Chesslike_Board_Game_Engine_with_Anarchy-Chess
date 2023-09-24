#ifndef KNOOK_H
#define KNOOK_H

#include "ChessPiece.h"

namespace chess {
    using namespace logic;

    /* A chess piece with a value of 9.0 that moves along L-shaped paths and diagonals*/
    class Knook : public ChessPiece
    {
        public:
            /* 
            * Inherit the ChessPiece constructors 
            */
            using ChessPiece::ChessPiece;

            /*
            * Returns the value of the piece, 9.0
            */
            double getValue() override;

            /*
            * Returns the knook's ID
            */
            ID getID() override;

        private:
            /*
            * Returns moves along L-shaped paths and diagonals that are not blocked
            */
            std::vector<Move> generateMoves(GameState& gameState) override;

            /*
            * Returns moves along L-shaped paths and diagonals that are not blocked
            */
            std::vector<Move> generateAttackingMoves(GameState& gameState) override;
    };

}
#endif