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
            /*
             * Checks if moving to the position will put the current player into 
             * check and, if it does not, adds the position to the move
             * 
             * Returns:
             * - true if the position was added to the move
             * - false if the position was not added to the move
            */
            bool chessAddToMove(Move::position position, Move& move, ChessGameState& chessState);
    };

}
#endif