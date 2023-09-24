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

            /*
            * Returns the bishop's ID
            */
            ID getID() override;

        private:
            /*
            * Adds all of the bishop's moves, which include diagonal movement
            * and its special move: Il Vaticano
            */
            std::vector<Move> generateMoves(GameState& gameState) override;

            /*
            * Adds all of the bishops attacks
            */
            std::vector<Move> generateAttackingMoves(GameState& gameState) override;

            /*
            * Adds moves along diagonals that are not blocked
            */
            void addStandardMoves(std::vector<Move>& moves, ChessGameState& chessState);

            /*
            * Allows bishops to switch places and destroy pieces in between if the
            * two bishops have two spaces between them and those spaces are inhabited by
            * pawns
            */
            void addIlVaticano(std::vector<Move>& moves, ChessGameState& chessState, bool attackOnly = false);
    };

}
#endif