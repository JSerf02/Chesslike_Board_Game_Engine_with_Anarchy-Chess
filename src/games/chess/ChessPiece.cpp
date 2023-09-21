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
        // Get a reference to the game board
        GameBoard* board = chessState.getBoard();

        // Simulate moving the piece and see if it will put the player in check
        if (board->simulateMovePiece(getPosition(), position)) {
            return false; // The move is impossible so it cannot be added
        }
        bool isSafe = !chessState.isInCheck();
        board->revertSimulation();

        // Add the move only if it does not put the player in check
        if(isSafe) {
            move.addPosition(position);
        }
        return isSafe;
    }

}