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
#include "Pawn.h"

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

    // Make sure capturing works
    CHECK(chessState->movePiece(std::make_pair(4, 4), std::make_pair(3, 3)));
}

TEST_CASE("Bishop: Value")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Get a bishop from the chess board
    Piece* bishop = board->getPiece(3, 1);

    // Make sure the bishop's value is 3.0
    CHECK(bishop->getValue() == 3.0);

    // Make sure the bishop's ID is correctly set
    CHECK(bishop->getID() == BISHOP_ID);
}

TEST_CASE("Bishop: Il Vaticano Success")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Create the following board state:
    // https://lichess.org/editor/7K/2B4k/2p5/2p5/2BppB2/8/8/8_w_-_-_0_1?color=white
    ChessPiece* bishop1 = new Bishop(Player::white, std::make_pair(3, 4));
    ChessPiece* bishop2 = new Bishop(Player::white, std::make_pair(6, 4));
    ChessPiece* bishop3 = new Bishop(Player::white, std::make_pair(3, 7));
    ChessPiece* pawn1   = new Pawn(  Player::black, std::make_pair(4, 4));
    ChessPiece* pawn2   = new Pawn(  Player::black, std::make_pair(5, 4));
    ChessPiece* pawn3   = new Pawn(  Player::black, std::make_pair(3, 5));
    ChessPiece* pawn4   = new Pawn(  Player::black, std::make_pair(3, 6));
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 8));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 7));
    board->addPieces({
        bishop1, bishop2, bishop3, pawn1, pawn2, pawn3, pawn4, whiteKing, blackKing
    });

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure Il Vaticano moves work in all directions
    CHECK(chessState->canMovePiece(std::make_pair(3, 4), std::make_pair(6, 4)));
    CHECK(chessState->canMovePiece(std::make_pair(6, 4), std::make_pair(3, 4)));
    CHECK(chessState->canMovePiece(std::make_pair(3, 4), std::make_pair(3, 7)));
    CHECK(chessState->canMovePiece(std::make_pair(3, 7), std::make_pair(3, 4)));

    // Make sure Il Vaticano has a higher priority than regular moves
    CHECK(!chessState->canMovePiece(std::make_pair(3, 4), std::make_pair(4, 5)));
    
    // Make sure the pawns between the bishops are attacked in all scenarios
    CHECK(chessState->isAttacked(Player::black, std::make_pair(4, 4)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(5, 4)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(3, 5)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(3, 6)));

    // Perform Il Vaticano and make sure the pawns between the bishops are destroyed
    CHECK(chessState->movePiece(std::make_pair(3, 4), std::make_pair(6, 4)));
    CHECK(board->unoccupiedOnBoard(std::make_pair(4, 4)));
    CHECK(board->unoccupiedOnBoard(std::make_pair(5, 4)));

    // Make sure the bishops swapped and didn't get captured somehow
    CHECK(board->occupiedOnBoard(std::make_pair(3, 4)));
}

TEST_CASE("Bishop: Il Vaticano Failure")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Create the following board state:
    // https://lichess.org/editor/7K/2B4k/2p5/2p5/2BppB2/8/8/8_w_-_-_0_1?color=white
    ChessPiece* bishop1 = new Bishop(Player::white, std::make_pair(3, 4));
    ChessPiece* bishop2 = new Bishop(Player::white, std::make_pair(6, 4));
    ChessPiece* bishop3 = new Bishop(Player::white, std::make_pair(3, 7));
    ChessPiece* pawn  = new Pawn(Player::black, std::make_pair(4, 4));
    ChessPiece* test1 = new TestChessPiece(Player::black, std::make_pair(5, 4));
    ChessPiece* test2 = new TestChessPiece(Player::black, std::make_pair(3, 5));
    ChessPiece* test3 = new TestChessPiece(Player::black, std::make_pair(3, 6));
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 8));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 7));
    board->addPieces({
        bishop1, bishop2, bishop3, pawn, test1, test2, test3, whiteKing, blackKing
    });

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure Il Vaticano moves does not work in all directions
    CHECK(!chessState->canMovePiece(std::make_pair(3, 4), std::make_pair(6, 4)));
    CHECK(!chessState->canMovePiece(std::make_pair(6, 4), std::make_pair(3, 4)));
    CHECK(!chessState->canMovePiece(std::make_pair(3, 4), std::make_pair(3, 7)));
    CHECK(!chessState->canMovePiece(std::make_pair(3, 7), std::make_pair(3, 4)));

    // Make sure the pieces between the bishops are attacked in all scenarios
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(4, 4)));
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(5, 4)));
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(3, 5)));
    CHECK(!chessState->isAttacked(Player::black, std::make_pair(3, 6)));
}