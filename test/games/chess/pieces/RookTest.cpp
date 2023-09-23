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
#include "Rook.h"

using namespace logic;
using namespace chess;
using namespace testing;

using Player = Piece::Player;

TEST_CASE("Rook: Horizontal and Vertical Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a rook to the board on D4
    ChessPiece* rook = new Rook(Player::white, std::make_pair(4, 4));
    board->addPiece(rook);

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure you cannot move a rook off the board
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(4, 0)));
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(0, 4)));
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(4, 9)));
    CHECK(!chessState->canMovePiece(std::make_pair(4, 4), std::make_pair(9, 4)));

    // Store all valid positions
    // A position is valid if it is along the horizontal or vertical of the rook
    std::set<Move::position> validPositions {};
    for(int i = 1; i <= 8; i++) {
        if(i == 4) {
            continue;
        }
        validPositions.insert(std::make_pair(4, i));
        validPositions.insert(std::make_pair(i, 4));
    }

    // Make sure the rook can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, rook->getPosition(), validPositions);
}

TEST_CASE("Rook: Blocked Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a rook to the board on D4 and 4 other test pieces on D3, B4, D7, and H4
    ChessPiece* rook       = new Rook(          Player::white, std::make_pair(4, 4));
    ChessPiece* downBlock  = new TestChessPiece(Player::white, std::make_pair(4, 3));
    ChessPiece* leftBlock  = new TestChessPiece(Player::white, std::make_pair(2, 4));
    ChessPiece* upBlock    = new TestChessPiece(Player::white, std::make_pair(4, 7));
    ChessPiece* rightBlock = new TestChessPiece(Player::white, std::make_pair(8, 4));
    board->addPieces({rook, downBlock, leftBlock, upBlock, rightBlock});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all valid positions
    // A position is valid if it is horizontal or vertical and it is not blocked
    // by another piece
    std::set<Move::position> validPositions {
        std::make_pair(3, 4),
        std::make_pair(4, 5),
        std::make_pair(4, 6),
        std::make_pair(5, 4),
        std::make_pair(6, 4),
        std::make_pair(7, 4)
    };

    // Make sure the rook can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, rook->getPosition(), validPositions);
}

TEST_CASE("Rook: Attack")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a rook to the board on D4 and 5 other test pieces on D3, D2, B4, D7, and H4
    ChessPiece* rook             = new Rook(          Player::white, std::make_pair(4, 4));
    ChessPiece* downEnemy        = new TestChessPiece(Player::black, std::make_pair(4, 3));
    ChessPiece* downBlockedEnemy = new TestChessPiece(Player::black, std::make_pair(4, 2));
    ChessPiece* leftEnemy        = new TestChessPiece(Player::black, std::make_pair(2, 4));
    ChessPiece* upBlock          = new TestChessPiece(Player::white, std::make_pair(4, 7));
    ChessPiece* rightBlock       = new TestChessPiece(Player::white, std::make_pair(8, 4));
    board->addPieces({rook, downEnemy, downBlockedEnemy, leftEnemy, upBlock, rightBlock});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all attacked positions
    // A position is attacked if it is horizontal or vertical, it does not contain
    // a piece of the same color, and it is not blocked by another piece
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

    // Make sure the rook attacks only the valid attack positions
    TestChessHelpers::testPieceAttacks(chessState, Player::black, attackPositions);

    // Make sure capturing works
    CHECK(chessState->movePiece(std::make_pair(4, 4), std::make_pair(4, 3)));
}

TEST_CASE("Rook: Value")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Get a rook from the chess board
    Piece* rook = board->getPiece(1, 1);

    // Make sure the rook's value is 5.0
    CHECK(rook->getValue() == 5.0);

    // Make sure the rook's ID is correctly set
    CHECK(rook->getID() == ROOK_ID);
}
