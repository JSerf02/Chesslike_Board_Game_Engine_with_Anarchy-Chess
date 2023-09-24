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
            * Returns the king's ID
            */
            ID getID() override;

        private:
            /*
            * Returns moves that are 1 distance away that are not blocked and also 
            * the king's special move: castling
            */
            std::vector<Move> generateMoves(GameState& gameState) override;

            /*
            * Returns moves that are 1 distance away that are not blocked
            */
            std::vector<Move> generateAttackingMoves(GameState& gameState) override;

            /*
            * Adds moves that are 1 distance away that are not blocked
            */
            void addStandardMoves(std::vector<Move>& moves, ChessGameState& chessState);

            /*
            * Brings the king and the king's-side rook closer together and swaps 
            * their positions
            */
            void addKingsideCastle(std::vector<Move>& moves, ChessGameState& chessState);

            /*
            * Brings the king and the queen's-side rook closer together and swaps
            * their positions
            */
            void addQueensideCastle(std::vector<Move>& moves, ChessGameState& chessState);
    };

}
#endif