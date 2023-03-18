#include "ChessPiece.h"
#include "ChessGameState.h"
#include "Piece.h"
#include "Move.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    // See ChessPiece.h
    bool ChessPiece::chessAddToMove(Move::position position, Move& move, ChessGameState& chessState)
    {
        return true;
    }

}