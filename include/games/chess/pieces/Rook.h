#ifndef ROOK_H
#define ROOK_H

#include "ChessPiece.h"

namespace chess {
    using namespace logic;

    /* A chess piece with a value of 5.0 that moves horizontally and vertically */
    class Rook : public ChessPiece
    {
        public:
            /* 
            * Inherit the ChessPiece constructors 
            */
            using ChessPiece::ChessPiece;

            /*
            * Returns the value of the piece, 5.0
            */
            double getValue() override;

            /*
            * Returns the rook's ID
            */
            ID getID() override;

        private:
            /*
            * Returns moves along the horizontal and vertical that are not blocked
            */
            std::vector<Move> generateMoves(GameState& gameState) override;

            /*
            * Returns moves along the horizontal and vertical that are not blocked
            */
            std::vector<Move> generateAttackingMoves(GameState& gameState) override;
    };

}
#endif