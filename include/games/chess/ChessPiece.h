#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "ChessGameState.h"
#include "Piece.h"
#include "Move.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    class ChessPiece : public Piece
    {
        public:
            /* Inherit the constructure */
            using Piece::Piece;
            
            /*
             * Checks if moving to the position will put the current player into 
             * check and, if it does not, adds the position to the move
             * 
             * Returns:
             * - true if the position was added to the move
             * - false if the position was not added to the move
            */
            bool chessAddToMove(Move::position position, Move& move, ChessGameState& chessState);

            /*
             * Returns whether or not this piece is the king. Note that each player
             * must have exactly one king in order for the game to function!
            */
            virtual bool isKing() { return false; }
    };

}
#endif