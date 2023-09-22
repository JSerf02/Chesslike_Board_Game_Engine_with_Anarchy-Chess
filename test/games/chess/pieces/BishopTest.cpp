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
#include "Bishop.h"

using namespace logic;
using namespace chess;
using namespace testing;

using Player = Piece::Player;

TEST_CASE("Bishop: Diagonal Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a bishop to the board on D4
    ChessPiece* bishop = new Bishop(Player::white, std::make_pair(4, 4));
    board->addPiece(bishop);

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure you cannot move a bishop off the board
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(0, 0)));
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(9, 9)));
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(0, 8)));
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(8, 0)));

    // Store all valid positions
    // A position is valid if it is along either diagonal
    std::set<Move::position> validPositions {};
    for(int i = 1; i <= 8; i++) {
        if(i == 4) {
            continue;
        }
        validPositions.insert(std::make_pair(i, i));
        validPositions.insert(std::make_pair(i, 8 - i));
    }

    // Make sure the bishop can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, bishop->getPosition(), validPositions);
}

TEST_CASE("Bishop: Blocked Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a bishop to the board on D4 and 4 test pieces on C3, B6, H8, and G1
    ChessPiece* bishop     = new Bishop(        Player::white, std::make_pair(4, 4));
    ChessPiece* downBlock  = new TestChessPiece(Player::white, std::make_pair(3, 3));
    ChessPiece* leftBlock  = new TestChessPiece(Player::white, std::make_pair(2, 6));
    ChessPiece* upBlock    = new TestChessPiece(Player::white, std::make_pair(8, 8));
    ChessPiece* rightBlock = new TestChessPiece(Player::white, std::make_pair(7, 1));
    board->addPieces({bishop, downBlock, leftBlock, upBlock, rightBlock});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all valid positions
    // A position is valid if it is along a diagonal and it is not blocked
    // by another piece
    std::set<Move::position> validPositions {
        std::make_pair(3, 5),
        std::make_pair(5, 5),
        std::make_pair(6, 6),
        std::make_pair(7, 7),
        std::make_pair(5, 3),
        std::make_pair(6, 2)
    };

    // Make sure the bishop can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, bishop->getPosition(), validPositions);
}

TEST_CASE("Bishop: Attack")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a bishop to the board on D4 and 5 test pieces on C3, B6, H8, and G1
    ChessPiece* bishop           = new Bishop(Player::white, std::make_pair(4, 4));
    ChessPiece* downEnemy        = new TestChessPiece(Player::black, std::make_pair(3, 3));
    ChessPiece* downBlockedEnemy = new TestChessPiece(Player::black, std::make_pair(2, 2));
    ChessPiece* leftEnemy        = new TestChessPiece(Player::black, std::make_pair(2, 6));
    ChessPiece* upBlock          = new TestChessPiece(Player::white, std::make_pair(8, 8));
    ChessPiece* rightBlock       = new TestChessPiece(Player::white, std::make_pair(7, 1));
    board->addPieces({bishop, downEnemy, downBlockedEnemy, leftEnemy, upBlock, rightBlock});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all attacked positions
    // A position is attacked if it is along a diagonal, it does not contain
    // a piece of the same color, and it is not blocked by another piece
    std::set<Move::position> attackPositions {
        std::make_pair(3, 3),
        std::make_pair(3, 5),
        std::make_pair(2, 6),
        std::make_pair(5, 5),
        std::make_pair(6, 6),
        std::make_pair(7, 7),
        std::make_pair(5, 3),
        std::make_pair(6, 2)
    };

    // Make sure the bishop attacks only the valid attack positions
    TestChessHelpers::testPieceAttacks(chessState, Player::black, attackPositions);
}

TEST_CASE("Bishop: Value")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Get a bishop from the chess board
    Piece* bishop = board->getPiece(3, 1);

    // Make sure the bishop's value is 3.0
    CHECK(bishop->getValue() == 3.0);
}
