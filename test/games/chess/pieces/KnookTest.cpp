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
#include "Knook.h"

using namespace logic;
using namespace chess;
using namespace testing;

using Player = Piece::Player;

TEST_CASE("Knook: Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a knook to the board on D4
    ChessPiece* knook = new Knook(Player::white, std::make_pair(4, 4));
    board->addPiece(knook);

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
    for(int i = 1; i <= 8; i++) {
        if(i == 4) {
            continue;
        }
        validPositions.insert(std::make_pair(i, i));
        validPositions.insert(std::make_pair(i, 8 - i));
    }

    // Make sure the knook can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, knook->getPosition(), validPositions);
}

TEST_CASE("Knook: Blocked Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a knook to the board on D4
    ChessPiece* knook = new Knook(Player::white, std::make_pair(4, 4));
    board->addPiece(knook);

    // Add white pieces at every possible position the knook can move to in an L except 1
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

    // Add 4 test pieces on C3, B6, H8, and G1
    ChessPiece* downBlock  = new TestChessPiece(Player::white, std::make_pair(3, 3));
    ChessPiece* leftBlock  = new TestChessPiece(Player::white, std::make_pair(2, 6));
    ChessPiece* upBlock    = new TestChessPiece(Player::white, std::make_pair(8, 8));
    ChessPiece* rightBlock = new TestChessPiece(Player::white, std::make_pair(7, 1));
    board->addPieces({downBlock, leftBlock, upBlock, rightBlock});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all valid positions
    // In this case, the only valid position is the one space that isn't blocked
    std::set<Move::position> validPositions {
        std::make_pair(5, 6),
        std::make_pair(3, 5),
        std::make_pair(5, 5),
        std::make_pair(6, 6),
        std::make_pair(7, 7),
        std::make_pair(5, 3),
        std::make_pair(6, 2)
    };

    // Make sure the knook can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, knook->getPosition(), validPositions);
}

TEST_CASE("Knook: Attack")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a knook to the board on D4
    ChessPiece* knook = new Knook(Player::white, std::make_pair(4, 4));
    board->addPiece(knook);

    // Add white pieces at every possible position the knook can move to except 1
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

    // Add 5 test pieces on C3, B2, B6, H8, and G1
    ChessPiece* downEnemy        = new TestChessPiece(Player::black, std::make_pair(3, 3));
    ChessPiece* downBlockedEnemy = new TestChessPiece(Player::black, std::make_pair(2, 2));
    ChessPiece* leftEnemy        = new TestChessPiece(Player::black, std::make_pair(2, 6));
    ChessPiece* upBlock          = new TestChessPiece(Player::white, std::make_pair(8, 8));
    ChessPiece* rightBlock       = new TestChessPiece(Player::white, std::make_pair(7, 1));
    board->addPieces({downEnemy, downBlockedEnemy, leftEnemy, upBlock, rightBlock});

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
        std::make_pair(2, 5),
        std::make_pair(3, 3),
        std::make_pair(3, 5),
        std::make_pair(2, 6),
        std::make_pair(5, 5),
        std::make_pair(6, 6),
        std::make_pair(7, 7),
        std::make_pair(5, 3),
        std::make_pair(6, 2)
    };

    // Make sure the knook attacks only the valid attack positions
    TestChessHelpers::testPieceAttacks(chessState, Player::black, attackPositions);

    // Make sure capturing works
    CHECK(chessState->movePiece(std::make_pair(4, 4), std::make_pair(6, 5)));
}

TEST_CASE("Knook: Value")
{
    // Create a knook
    Piece* knook = new Knook(Player::white, std::make_pair(4, 4));

    // Make sure the knook's value is 9.0
    CHECK(knook->getValue() == 9.0);

    // Make sure the knook's ID is correctly set
    CHECK(knook->getID() == KNOOK_ID);
}
