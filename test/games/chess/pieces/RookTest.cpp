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

TEST_CASE("Rook: 4-Directional Movement")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Remove the white a pawn so the rook will be able to move upward initially
    Piece* aPawn = board->getPiece(std::make_pair(1, 2));
    CHECK(board->removePiece(aPawn->getPosition()));

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure you cannot move a rook off the board
    CHECK(!chessState->canMovePiece(std::make_pair(1, 1), std::make_pair(0, 0)));

    // Rook to A4 (test upward movement)
    CHECK(chessState->movePiece(std::make_pair(1, 1), std::make_pair(1, 4)));

    // Pawn to H6
    CHECK(chessState->movePiece(std::make_pair(8, 7), std::make_pair(8, 6)));

    // Rook to H4 (test rightward movement)
    CHECK(chessState->movePiece(std::make_pair(1, 4), std::make_pair(8, 4)));

    // Pawn to H5
    CHECK(chessState->movePiece(std::make_pair(8, 6), std::make_pair(8, 5)));

    // Rook to H3 (test downward movement)
    CHECK(chessState->movePiece(std::make_pair(8, 4), std::make_pair(8, 3)));

    // Pawn to H4
    CHECK(chessState->movePiece(std::make_pair(8, 5), std::make_pair(8, 4)));

    // Rook to C3 (test leftward movement)
    CHECK(chessState->movePiece(std::make_pair(8, 3), std::make_pair(3, 3)));

    // Pawn to H3
    CHECK(chessState->movePiece(std::make_pair(8, 4), std::make_pair(8, 3)));

    // Rook to C5
    CHECK(chessState->movePiece(std::make_pair(3, 3), std::make_pair(3, 5)));

    // Make sure the rook cannot move to diagonals
    CHECK(!chessState->canMovePiece(std::make_pair(3, 5), std::make_pair(2, 6))); // B6
    CHECK(!chessState->canMovePiece(std::make_pair(3, 5), std::make_pair(4, 6))); // D6
    CHECK(!chessState->canMovePiece(std::make_pair(3, 5), std::make_pair(2, 4))); // B4
    CHECK(!chessState->canMovePiece(std::make_pair(3, 5), std::make_pair(1, 3))); // A3
    CHECK(!chessState->canMovePiece(std::make_pair(3, 5), std::make_pair(4, 4))); // D4
    CHECK(!chessState->canMovePiece(std::make_pair(3, 5), std::make_pair(5, 3))); // E3

    // Make sure the rook cannot move to other random spaces
    CHECK(!chessState->canMovePiece(std::make_pair(3, 5), std::make_pair(8, 6))); // H6
    CHECK(!chessState->canMovePiece(std::make_pair(3, 5), std::make_pair(7, 4))); // G4
    CHECK(!chessState->canMovePiece(std::make_pair(3, 5), std::make_pair(1, 4))); // A4
    CHECK(!chessState->canMovePiece(std::make_pair(3, 5), std::make_pair(5, 8))); // E8
}

TEST_CASE("Rook: Blocked Movement")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Arrange the pieces so the Rook is blocked in all directions
    CHECK(board->movePiece(std::make_pair(1, 1), std::make_pair(4, 4))); // A Rook to D4
    CHECK(board->movePiece(std::make_pair(3, 2), std::make_pair(3, 4))); // C Pawn to C4
    CHECK(board->movePiece(std::make_pair(4, 2), std::make_pair(4, 3))); // D Pawn to D3
    CHECK(board->movePiece(std::make_pair(5, 2), std::make_pair(4, 5))); // E Pawn to D5
    CHECK(board->movePiece(std::make_pair(6, 2), std::make_pair(6, 4))); // F Pawn to F4
    

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Ensure that the Rook cannot move onto another piece of the same color
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(3, 4))); // C4 
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(4, 3))); // D3
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(4, 5))); // D5
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(6, 4))); // F4

    // Ensure that the rook cannot jump over its own pieces
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(2, 4))); // B4
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(4, 2))); // D2
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(4, 6))); // D6
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(7, 4))); // G4
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(8, 4))); // H4

    // Make sure unblocked moves still work
    CHECK(chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(5, 4))); // E4
}

TEST_CASE("Rook: Attack")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Arrange the pieces so the Rook is blocked in all directions
    CHECK(board->movePiece(std::make_pair(1, 1), std::make_pair(4, 4))); // A Rook to D4
    CHECK(board->movePiece(std::make_pair(3, 7), std::make_pair(3, 4))); // C Pawn to C4
    CHECK(board->movePiece(std::make_pair(5, 7), std::make_pair(4, 5))); // E Pawn to D5
    CHECK(board->movePiece(std::make_pair(6, 7), std::make_pair(6, 4))); // F Pawn to F4
    

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Ensure that the Rook is attacking the correct spaces
    CHECK(chessState->isAttacked(Player::black, std::make_pair(3, 4))); // C4 
    CHECK(chessState->isAttacked(Player::black, std::make_pair(4, 5))); // D5
    CHECK(chessState->isAttacked(Player::black, std::make_pair(5, 4))); // E4

    // Ensure that the rook is not attacking spaces due to blocks
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(2, 4))); // B4
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(4, 6))); // D6
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(7, 4))); // G4
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(8, 4))); // H4

    // Ensure that the rook is not attacking its own color's piece
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(4, 2))); // D2

    // Ensure that the rook is not attacking random other spaces
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(8, 6))); // H6
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(7, 5))); // G5
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(1, 5))); // A5
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(5, 8))); // E8
}

TEST_CASE("Rook: Value")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Get a rook from the chess board
    Piece* rook = board->getPiece(1, 1);

    // Make sure the rook's value is 5.0
    CHECK(rook->getValue() == 5.0);
}
