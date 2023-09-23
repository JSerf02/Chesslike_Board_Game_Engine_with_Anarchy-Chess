#ifndef PAWN_H
#define PAWN_H

#include "ChessPiece.h"

namespace chess {
    using namespace logic;

    /* A chess piece with a value of 1.0 that moves straight, attacks on diagonals,
       can promote to other pieces, and can boost an extra space forwards on its 
       first turn */
    class Pawn : public ChessPiece
    {
        public:
            /* 
            * Inherit the ChessPiece constructors 
            */
            using ChessPiece::ChessPiece;

            /*
            * Returns the value of the piece, 1.0
            */
            double getValue() override;

            /*
            * Returns the pawn's ID
            */
            ID getID() override;

            /*
            * Whether the pawn has already moved this game
            */
            bool alreadyMoved{};

            /*
            * The turn that the pawn boosted
            * - Used for determining whether En Passant is possible
            */
            int boostTurn{};

            /*
            * Returns whether the pawn can boost
            * - The pawn can boost if it has not moved previously
            */
            bool canBoost();

        private:
            /*
            * Returns moves that are one space ahead, diagonal when the space contains
            * an enemy piece, and two spaces ahead if the pawn has not yet moved
            */
            std::vector<Move> generateMoves(GameState& gameState) override;

            /*
            * Returns moves that are one space ahead, diagonal when the space contains
            * an enemy piece, and two spaces ahead if the pawn has not yet moved
            */
            std::vector<Move> generateAttackingMoves(GameState& gameState) override;

            /*
            * Returns whether the pawn boosted on the previoius turn
            */
            bool justBoosted();
    };
}
#endif