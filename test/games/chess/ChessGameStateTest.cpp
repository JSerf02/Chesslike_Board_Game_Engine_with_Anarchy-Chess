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
    CHECK(whiteKing->getPosition() == std::make_pair(5, 1));
    CHECK(blackKing->getPosition() == std::make_pair(5, 8));
}

TEST_CASE("Chess Game State - Check")
{
    // Initialize the ChessGameState and make sure it isn't in check by default
    ChessGameState chessState{};
    CHECK(!chessState.isInCheck());
    CHECK(!chessState.isInCheck(Player::black));
    
    // Perform the legendary 2-move checkmate
    CHECK(chessState.movePiece(std::make_pair(6, 2), std::make_pair(6, 3))); // Pawn to F3
    CHECK(!chessState.isInCheck());
    CHECK(chessState.movePiece(std::make_pair(5, 7), std::make_pair(5, 5))); // Pawn to E5
    CHECK(!chessState.isInCheck());
    CHECK(chessState.movePiece(std::make_pair(7, 2), std::make_pair(7, 4))); // Pawn to G4
    CHECK(!chessState.isInCheck());
    CHECK(chessState.movePiece(std::make_pair(4, 8), std::make_pair(8, 4))); // Queen to H4

    // Make sure white is actually in check after this brilliant sequence of moves
    CHECK(chessState.isInCheck());
    CHECK(!chessState.isInCheck(Player::black));
}

TEST_CASE("Chess Game State - Checkmate")
{
    // Initialize the ChessGameState and make sure it isn't in check by default
    ChessGameState chessState{};
    CHECK(!chessState.isInCheckmate());
    
    // Perform the legendary 2-move checkmate
    CHECK(chessState.movePiece(std::make_pair(6, 2), std::make_pair(6, 3))); // Pawn to F3
    CHECK(!chessState.isInCheck());
    CHECK(chessState.movePiece(std::make_pair(5, 7), std::make_pair(5, 5))); // Pawn to E5
    CHECK(!chessState.isInCheck());
    CHECK(chessState.movePiece(std::make_pair(7, 2), std::make_pair(7, 4))); // Pawn to G4
    CHECK(!chessState.isInCheck());
    CHECK(chessState.movePiece(std::make_pair(4, 8), std::make_pair(8, 4))); // Queen to H4

    // Make sure white is actually in checkmate after this brilliant sequence of moves
    CHECK(chessState.isInCheckmate());
}

TEST_CASE("Chess Game State: Initialize Object With Imported Board")
{
    // Initialize a ChessBoard object
    ChessBoard* board = new ChessBoard();

    // Put the white king in checkmate 
    board->movePiece(std::make_pair(6, 2), std::make_pair(6, 3)); // Pawn to F3
    board->movePiece(std::make_pair(7, 2), std::make_pair(7, 4)); // Pawn to G4
    board->movePiece(std::make_pair(4, 8), std::make_pair(8, 4)); // Queen to H4

    // Initialize a chess game state using the preexisting board
    ChessGameState* chessState = new ChessGameState(board);
    
    // Make sure the board is set to the correct reference
    CHECK(chessState->getBoard() == board);

    // Ensure the kings are set and returned properly
    Piece* whiteKing = chessState->getKing(Player::white);
    Piece* blackKing = chessState->getKing(Player::black);
    REQUIRE(whiteKing != nullptr);
    REQUIRE(blackKing != nullptr);
    CHECK(whiteKing->getPosition() == std::make_pair(5, 1));
    CHECK(blackKing->getPosition() == std::make_pair(5, 8));

    // Make sure checkmate is properly detected
    CHECK(chessState->isInCheckmate(Player::white));
}
