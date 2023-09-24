#include <vector>
#include <iostream>
#include <algorithm>
#include <set>

#include "doctest.h"
#include "TestChessHelpers.h"
#include "TestPieces.h"

#include "ChessPiece.h"
#include "ChessBoard.h"
#include "ChessGameState.h"
#include "Move.h"
#include "Piece.h"
#include "GameBoard.h"
#include "GameState.h"
#include "Knight.h"

using namespace logic;
using namespace chess;
using namespace testing;

using Player = Piece::Player;

TEST_CASE("Knight: L Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a knight to the board on D4
    ChessPiece* knight = new Knight(Player::white, std::make_pair(4, 4));
    board->addPiece(knight);

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all valid positions
    // A position is valid if it is along an L
    std::vector<Move::position> positionDeltas = {
        std::make_pair(1, 2),
        std::make_pair(2, 1),
        std::make_pair(-1, 2),
        std::make_pair(-2, 1),
        std::make_pair(1, -2),
        std::make_pair(2, -1),
        std::make_pair(-1, -2),
        std::make_pair(-2, -1)
    };
    std::set<Move::position> validPositions {};
    for(Move::position delta : positionDeltas) {
        validPositions.insert(std::make_pair(4 + delta.first, 4 + delta.second));
    }

    // Make sure the knight can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, knight->getPosition(), validPositions);
}

TEST_CASE("Knight: Blocked Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a knight to the board on D4
    ChessPiece* knight = new Knight(Player::white, std::make_pair(4, 4));
    board->addPiece(knight);

    // Add white pieces at every possible position the knight can move to except 1
    std::vector<Move::position> positionDeltas = {
        std::make_pair(2, 1),
        std::make_pair(-1, 2),
        std::make_pair(-2, 1),
        std::make_pair(1, -2),
        std::make_pair(2, -1),
        std::make_pair(-1, -2),
        std::make_pair(-2, -1)
    };
    for(Move::position delta : positionDeltas) {
        ChessPiece* curBlock = new TestChessPiece(Player::white, std::make_pair(4 + delta.first, 4 + delta.second));
        board->addPiece(curBlock);
    }

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all valid positions
    // In this case, the only valid position is the one space that isn't blocked
    std::set<Move::position> validPositions {
        std::make_pair(5, 6)
    };

    // Make sure the knight can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, knight->getPosition(), validPositions);
}

TEST_CASE("Knight: Attack")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a knight to the board on D4
    ChessPiece* knight = new Knight(Player::white, std::make_pair(4, 4));
    board->addPiece(knight);

    // Add white pieces at every possible position the knight can move to except 1
    std::vector<Move::position> positionDeltas = {
        std::make_pair(2, 1),
        std::make_pair(-1, 2),
        std::make_pair(-2, 1),
        std::make_pair(1, -2),
        std::make_pair(2, -1),
        std::make_pair(-1, -2),
        std::make_pair(-2, -1)
    };
    int count = 0; // Makes sure the first 3 pieces are black and the rest are white
    for(Move::position delta : positionDeltas) {
        ChessPiece* curBlock = new TestChessPiece(count < 3 ? Player::black : Player::white, std::make_pair(4 + delta.first, 4 + delta.second));
        count++;
        board->addPiece(curBlock);
    }

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all attacked positions
    // A position is attacked if it is along and L and not blocked by a piece of
    // the same color
    std::set<Move::position> attackPositions {
        std::make_pair(5, 6),
        std::make_pair(6, 5),
        std::make_pair(3, 6),
        std::make_pair(2, 5)
    };

    // Make sure the knight attacks only the valid attack positions
    TestChessHelpers::testPieceAttacks(chessState, Player::black, attackPositions);

    // Make sure capturing works
    CHECK(chessState->movePiece(std::make_pair(4, 4), std::make_pair(6, 5)));
}

TEST_CASE("Knight: Value")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Get a knight from the chess board
    Piece* knight = board->getPiece(2, 1);

    // Make sure the knight's value is 3.0
    CHECK(knight->getValue() == 3.0);

    // Make sure the knight's ID is correctly set
    CHECK(knight->getID() == KNIGHT_ID);
}
