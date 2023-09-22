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
#include "Queen.h"

using namespace logic;
using namespace chess;
using namespace testing;

using Player = Piece::Player;

TEST_CASE("Queen: Omnidirectional Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a queen to the board on D4
    ChessPiece* queen = new Queen(Player::white, std::make_pair(4, 4));
    board->addPiece(queen);

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure you cannot move a queen off the board
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(4, 0)));
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(0, 4)));
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(4, 9)));
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(9, 4)));

    // Store all valid positions
    // A position is valid if it is along any of the 8 cardinal directions
    std::set<Move::position> validPositions {};
    for(int i = 1; i <= 8; i++) {
        if(i == 4) {
            continue;
        }
        validPositions.insert(std::make_pair(i, i));
        validPositions.insert(std::make_pair(i, 8 - i));
        validPositions.insert(std::make_pair(4, i));
        validPositions.insert(std::make_pair(i, 4));
    }

    // Make sure the queen can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, queen->getPosition(), validPositions);
}

TEST_CASE("Queen: Diagonal Blocked Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a queen to the board on D4 and 4 test pieces on C3, B6, H8, and G1
    ChessPiece* queen      = new Queen(        Player::white, std::make_pair(4, 4));
    ChessPiece* downBlock  = new TestChessPiece(Player::white, std::make_pair(3, 3));
    ChessPiece* leftBlock  = new TestChessPiece(Player::white, std::make_pair(2, 6));
    ChessPiece* upBlock    = new TestChessPiece(Player::white, std::make_pair(8, 8));
    ChessPiece* rightBlock = new TestChessPiece(Player::white, std::make_pair(7, 1));
    board->addPieces({queen, downBlock, leftBlock, upBlock, rightBlock});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all valid positions
    // A position is valid if it is along any of the 8 cardinal directions
    std::set<Move::position> validPositions {
        std::make_pair(3, 5),
        std::make_pair(5, 5),
        std::make_pair(6, 6),
        std::make_pair(7, 7),
        std::make_pair(5, 3),
        std::make_pair(6, 2)
    };
    for(int i = 1; i <= 8; i++) {
        if(i == 4) {
            continue;
        }
        validPositions.insert(std::make_pair(4, i));
        validPositions.insert(std::make_pair(i, 4));
    }

    // Make sure the queen can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, queen->getPosition(), validPositions);
}

TEST_CASE("Queen: Horizontal and Vertical Blocked Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a queen to the board on D4 and 4 other test pieces on D3, B4, D7, and H4
    ChessPiece* queen      = new Queen(         Player::white, std::make_pair(4, 4));
    ChessPiece* downBlock  = new TestChessPiece(Player::white, std::make_pair(4, 3));
    ChessPiece* leftBlock  = new TestChessPiece(Player::white, std::make_pair(2, 4));
    ChessPiece* upBlock    = new TestChessPiece(Player::white, std::make_pair(4, 7));
    ChessPiece* rightBlock = new TestChessPiece(Player::white, std::make_pair(8, 4));
    board->addPieces({queen, downBlock, leftBlock, upBlock, rightBlock});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all valid positions
    // A position is valid if it is along any of the 8 cardinal directions
    std::set<Move::position> validPositions {
        std::make_pair(3, 4),
        std::make_pair(4, 5),
        std::make_pair(4, 6),
        std::make_pair(5, 4),
        std::make_pair(6, 4),
        std::make_pair(7, 4)
    };
    for(int i = 1; i <= 8; i++) {
        if(i == 4) {
            continue;
        }
        validPositions.insert(std::make_pair(i, i));
        validPositions.insert(std::make_pair(i, 8 - i));
    }

    // Make sure the queen can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, queen->getPosition(), validPositions);
}

TEST_CASE("Queen: Diagonal Attack")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a queen to the board on D4 and 5 test pieces on C3, B6, H8, and G1
    ChessPiece* queen            = new Queen(Player::white, std::make_pair(4, 4));
    ChessPiece* downEnemy        = new TestChessPiece(Player::black, std::make_pair(3, 3));
    ChessPiece* downBlockedEnemy = new TestChessPiece(Player::black, std::make_pair(2, 2));
    ChessPiece* leftEnemy        = new TestChessPiece(Player::black, std::make_pair(2, 6));
    ChessPiece* upBlock          = new TestChessPiece(Player::white, std::make_pair(8, 8));
    ChessPiece* rightBlock       = new TestChessPiece(Player::white, std::make_pair(7, 1));
    board->addPieces({queen, downEnemy, downBlockedEnemy, leftEnemy, upBlock, rightBlock});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all attacked positions
    // A position is valid if it is along any of the 8 cardinal directions
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
    for(int i = 1; i <= 8; i++) {
        if(i == 4) {
            continue;
        }
        attackPositions.insert(std::make_pair(4, i));
        attackPositions.insert(std::make_pair(i, 4));
    }

    // Make sure the queen attacks only the valid attack positions
    TestChessHelpers::testPieceAttacks(chessState, Player::black, attackPositions);
}

TEST_CASE("Queen: Horizontal and Vertical Attack")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a queen to the board on D4 and 5 other test pieces on D3, D2, B4, D7, and H4
    ChessPiece* queen            = new Queen(         Player::white, std::make_pair(4, 4));
    ChessPiece* downEnemy        = new TestChessPiece(Player::black, std::make_pair(4, 3));
    ChessPiece* downBlockedEnemy = new TestChessPiece(Player::black, std::make_pair(4, 2));
    ChessPiece* leftEnemy        = new TestChessPiece(Player::black, std::make_pair(2, 4));
    ChessPiece* upBlock          = new TestChessPiece(Player::white, std::make_pair(4, 7));
    ChessPiece* rightBlock       = new TestChessPiece(Player::white, std::make_pair(8, 4));
    board->addPieces({queen, downEnemy, downBlockedEnemy, leftEnemy, upBlock, rightBlock});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all attacked positions
    // A position is valid if it is along any of the 8 cardinal directions
    std::set<Move::position> attackPositions {
        std::make_pair(4, 3),
        std::make_pair(3, 4),
        std::make_pair(2, 4),
        std::make_pair(4, 5),
        std::make_pair(4, 6),
        std::make_pair(5, 4),
        std::make_pair(6, 4),
        std::make_pair(7, 4)
    };
    for(int i = 1; i <= 8; i++) {
        if(i == 4) {
            continue;
        }
        attackPositions.insert(std::make_pair(i, i));
        attackPositions.insert(std::make_pair(i, 8 - i));
    }

    // Make sure the queen attacks only the valid attack positions
    TestChessHelpers::testPieceAttacks(chessState, Player::black, attackPositions);
}

TEST_CASE("Queen: Value")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Get a queen from the chess board
    Piece* queen = board->getPiece(4, 1);

    // Make sure the queen's value is 9.0
    CHECK(queen->getValue() == 9.0);
}
