#include <vector>
#include <iostream>
#include <algorithm>

#include "doctest.h"
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "ChessGameState.h"
#include "Move.h"
#include "Piece.h"
#include "GameBoard.h"
#include "GameState.h"

using namespace logic;
using namespace chess;
using Player = Piece::Player;

TEST_CASE("Chess Piece: Add to Move")
{
    // Create a ChessGameState
    ChessGameState chessState{};

    // Make moves so moving the gPawn to G4 is checkmate and moving the gPawn to G3 is safe
    CHECK(chessState.movePiece(std::make_pair(2, 6), std::make_pair(3, 6))); // Pawn to F3
    CHECK(chessState.movePiece(std::make_pair(7, 5), std::make_pair(5, 5))); // Pawn to E5
    CHECK(chessState.movePiece(std::make_pair(2, 1), std::make_pair(3, 1))); // Pawn to A4
    CHECK(chessState.movePiece(std::make_pair(8, 4), std::make_pair(4, 8))); // Queen to H4

    // Get the current game board and get the G-Pawn from it
    GameBoard* board = chessState.getBoard();
    REQUIRE(board != nullptr);
    ChessPiece* gPawn = dynamic_cast<ChessPiece*>(board->getPiece(2, 7));
    REQUIRE(gPawn != nullptr);

    // Create a move
    Move move{};

    // Make sure adding pawn to G4 fails
    CHECK(gPawn->chessAddToMove(std::make_pair(4, 7), move, chessState) == false);
    CHECK(move.containsPosition(std::make_pair(3, 7)) == false);

    // Make sure adding pawn to G3 succeeds
    CHECK(gPawn->chessAddToMove(std::make_pair(3, 7), move, chessState));
    CHECK(move.containsPosition(std::make_pair(3, 7)));
}