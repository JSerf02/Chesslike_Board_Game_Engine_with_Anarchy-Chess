#ifndef KNIGHT_H
#define KNIGHT_H

#include "ChessPiece.h"

namespace chess {
    using namespace logic;

    /* A chess piece with a value of 3.0 that moves along L-shaped paths */
    class Knight : public ChessPiece
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

            /*
            * Returns the knight's ID
            */
            ID getID() override;

        private:
            /*
            * Returns moves along L-shaped paths that are not blocked and Knooklear
            * Fusion moves
            */
            std::vector<Move> generateMoves(GameState& gameState) override;

            /*
            * Returns moves along L-shaped paths that are not blocked
            */
            std::vector<Move> generateAttackingMoves(GameState& gameState) override;

            /*
            * Returns moves along L-shaped paths that are not blocked
            */
            std::vector<Move> generateStandardMoves(GameState& gameState);

            /*
            * Adds moves along L-shaped paths that are blocked by rooks
            * - These moves replace the rooks with Knooks
            */
            void addKnooklearFusion(std::vector<Move>& moves, GameState& gameState);
    };

}
#endif