#include <vector>
#include <iostream>
#include <algorithm>

#include "doctest.h"
#include "ChessGameState.h"
#include "Move.h"
#include "Piece.h"
#include "GameBoard.h"
#include "GameState.h"

using namespace logic;
using namespace chess;
using Player = Piece::Player;

TEST_CASE("Chess Game State: Initialize object")
{
    // Create a ChessGameState
    ChessGameState chessState{};

    // Get the chess board and make sure it properly returned
    GameBoard* chessBoard = chessState.getBoard();
    REQUIRE(chessBoard != nullptr);

    // Ensure the chess board returns the correct spaces are on the board
    CHECK(chessBoard->onBoard(1, 1));
    CHECK(chessBoard->onBoard(0, 0) == false);

    // Ensure the chess board has the proper pieces setup
    for(int i = 1; i <= 8; i++) {
        CHECK(chessBoard->occupiedOnBoard(i, 1));
        CHECK(chessBoard->occupiedOnBoard(i, 2));
        for(int j = 3; j <=6; j++){
            CHECK(chessBoard->unoccupiedOnBoard(i, j));
        }
        CHECK(chessBoard->occupiedOnBoard(i, 7));
        CHECK(chessBoard->occupiedOnBoard(i, 8));
    }

    // Ensure the kings are set and returned properly
    Piece* whiteKing = chessState.getKing(Player::white);
    Piece* blackKing = chessState.getKing(Player::black);
    REQUIRE(whiteKing != nullptr);
    REQUIRE(blackKing != nullptr);
    CHECK(whiteKing->getPosition() == std::make_pair(1, 5));
    CHECK(blackKing->getPosition() == std::make_pair(8, 5));
}

TEST_CASE("Chess Game State - Check")
{
    // Initialize the ChessGameState and make sure it isn't in check by default
    ChessGameState chessState{};
    CHECK(!chessState.isInCheck());
    CHECK(!chessState.isInCheck(Player::black));
    
    // Perform the legendary 2-move checkmate
    CHECK(chessState.movePiece(std::make_pair(2, 6), std::make_pair(3, 6))); // Pawn to F3
    CHECK(!chessState.isInCheck());
    CHECK(chessState.movePiece(std::make_pair(7, 5), std::make_pair(5, 5))); // Pawn to E5
    CHECK(!chessState.isInCheck());
    CHECK(chessState.movePiece(std::make_pair(2, 7), std::make_pair(4, 7))); // Pawn to G4
    CHECK(!chessState.isInCheck());
    CHECK(chessState.movePiece(std::make_pair(8, 4), std::make_pair(4, 8))); // Queen to H4
    CHECK(!chessState.isInCheck());

    // Make sure white is actually in check after this brilliant sequence of moves
    CHECK(chessState.isInCheck());
    CHECK(!chessState.isInCheck(Player::black));
}

TEST_CASE("Chess Game State - Will Move Cause Check")
{
    // Initialize the ChessGameState
    ChessGameState chessState{};

    // Make moves to create a position where white will be in check if white plays Pawn to G4
    // Make sure none of these moves will cause check 
    CHECK(!chessState.willMoveCauseCheck(std::make_pair(2, 6), std::make_pair(3, 6)));
    CHECK(chessState.movePiece(std::make_pair(2, 6), std::make_pair(3, 6))); // Pawn to F3

    CHECK(!chessState.willMoveCauseCheck(std::make_pair(7, 5), std::make_pair(5, 5)));
    CHECK(chessState.movePiece(std::make_pair(7, 5), std::make_pair(5, 5))); // Pawn to E5

    CHECK(!chessState.willMoveCauseCheck(std::make_pair(2, 1), std::make_pair(3, 1)));
    CHECK(chessState.movePiece(std::make_pair(2, 1), std::make_pair(3, 1))); // Pawn to A3

    CHECK(!chessState.willMoveCauseCheck(std::make_pair(2, 6), std::make_pair(3, 6)));
    CHECK(chessState.movePiece(std::make_pair(8, 4), std::make_pair(4, 8))); // Queen to H4

    // Make sure chessState returns that playing Pawn to G4 will put white into check
    // and make moving the piece does not work
    CHECK(chessState.willMoveCauseCheck(std::make_pair(2, 7), std::make_pair(4, 7)));
    CHECK(!chessState.movePiece(std::make_pair(2, 7), std::make_pair(4, 7))); // Pawn to G4
}

TEST_CASE("Chess Game State - Checkmate")
{
    // Initialize the ChessGameState and make sure it isn't in check by default
    ChessGameState chessState{};
    CHECK(!chessState.isInCheckmate());
    
    // Perform the legendary 2-move checkmate
    CHECK(chessState.movePiece(std::make_pair(2, 6), std::make_pair(3, 6))); // Pawn to F3
    CHECK(!chessState.isInCheckmate());
    CHECK(chessState.movePiece(std::make_pair(7, 5), std::make_pair(5, 5))); // Pawn to E5
    CHECK(!chessState.isInCheckmate());
    CHECK(chessState.movePiece(std::make_pair(2, 7), std::make_pair(4, 7))); // Pawn to G4
    CHECK(!chessState.isInCheckmate());
    CHECK(chessState.movePiece(std::make_pair(8, 4), std::make_pair(4, 8))); // Queen to H4

    // Make sure white is actually in checkmate after this brilliant sequence of moves
    CHECK(chessState.isInCheckmate());
}

