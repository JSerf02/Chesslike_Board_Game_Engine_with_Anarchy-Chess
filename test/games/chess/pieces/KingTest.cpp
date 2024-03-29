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
#include "King.h"
#include "Rook.h"

using namespace logic;
using namespace chess;
using namespace testing;

using Player = Piece::Player;

TEST_CASE("King: Omnidirectional Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add kings to the board on A1 and D4
    ChessPiece* whiteKing = new King(Player::white, std::make_pair(1, 1));
    ChessPiece* blackKing = new King(Player::black, std::make_pair(4, 4));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure you cannot move a king off the board
    CHECK(!chessState->canMovePiece(std::make_pair(1, 1), std::make_pair(1, 0)));
    CHECK(!chessState->canMovePiece(std::make_pair(1, 1), std::make_pair(0, 1)));

    // Move the white king so it is black's turn
    CHECK(chessState->movePiece(std::make_pair(1, 1), std::make_pair(1, 2)));

    // Store all valid positions
    // A position is valid if it is within one space of the king (the black king in this case)
    std::set<Move::position> validPositions {
        std::make_pair(3, 4),
        std::make_pair(3, 5),
        std::make_pair(4, 5),
        std::make_pair(5, 5),
        std::make_pair(5, 4),
        std::make_pair(5, 3),
        std::make_pair(4, 3),
        std::make_pair(3, 3)
    };

    // Make sure the black king can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, blackKing->getPosition(), validPositions);
}

TEST_CASE("King: Blocked Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add kings to the board on D4 and H8 and test pieces to every space around 
    // the D4 king except 1 space
    ChessPiece* whiteKing  = new King(          Player::white, std::make_pair(4, 4));
    ChessPiece* blackKing  = new King(          Player::black, std::make_pair(8, 8));
    ChessPiece* block1     = new TestChessPiece(Player::white, std::make_pair(3, 4));
    ChessPiece* block2     = new TestChessPiece(Player::white, std::make_pair(3, 5));
    ChessPiece* block3     = new TestChessPiece(Player::white, std::make_pair(4, 5));
    ChessPiece* block4     = new TestChessPiece(Player::white, std::make_pair(5, 5));
    ChessPiece* block5     = new TestChessPiece(Player::white, std::make_pair(5, 4));
    ChessPiece* block6     = new TestChessPiece(Player::white, std::make_pair(5, 3));
    ChessPiece* block7     = new TestChessPiece(Player::white, std::make_pair(4, 3));
    board->addPieces({whiteKing, blackKing, block1, block2, block3, block4, block5, block6, block7});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all valid positions
    // In this case, the only valid move is the only space not blocked
    std::set<Move::position> validPositions {
        std::make_pair(3, 3)
    };

    // Make sure the king can only move to valid positions
    TestChessHelpers::testPiecePositions(chessState, whiteKing->getPosition(), validPositions);
}

TEST_CASE("King: Attack")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add kings to the board on D4 and H8 and test pieces to every space around
    // the D4 king except for D5 and E5
    ChessPiece* whiteKing  = new King(          Player::white, std::make_pair(4, 4));
    ChessPiece* blackKing  = new King(          Player::black, std::make_pair(8, 8));
    ChessPiece* enemy1     = new TestChessPiece(Player::black, std::make_pair(3, 4));
    ChessPiece* enemy2     = new TestChessPiece(Player::black, std::make_pair(3, 5));
    ChessPiece* block1     = new TestChessPiece(Player::white, std::make_pair(5, 4));
    ChessPiece* block2     = new TestChessPiece(Player::white, std::make_pair(5, 3));
    ChessPiece* block3     = new TestChessPiece(Player::white, std::make_pair(4, 3));
    ChessPiece* block4     = new TestChessPiece(Player::white, std::make_pair(3, 3));
    board->addPieces({whiteKing, blackKing, enemy1, enemy2, block1, block2, block3, block4});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Store all attacked positions
    // The attacked positions are the pieces that are within one space of the 
    // king and that do not contain pieces of the same color
    std::set<Move::position> attackPositions {
        std::make_pair(3, 4),
        std::make_pair(3, 5),
        std::make_pair(4, 5),
        std::make_pair(5, 5)
    };

    // Make sure the king attacks only the valid attack positions
    TestChessHelpers::testPieceAttacks(chessState, Player::black, attackPositions);

    // Make sure capturing works
    CHECK(chessState->movePiece(std::make_pair(4, 4), std::make_pair(3, 4)));
}

TEST_CASE("King: Value and ID")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Get a king from the chess board
    Piece* king = board->getPiece(5, 1);

    // Make sure the king's value is 100.0
    CHECK(king->getValue() == 100.0);

    // Make sure the king's ID is correctly set
    CHECK(king->getID() == KING_ID);

    // Make sure the king is detected as a king
    CHECK(static_cast<ChessPiece*>(king)->isKing());
}

TEST_CASE("King: Castling")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add pieces so white can castle on both sides and black can queen-side castle
    ChessPiece* whiteKing = new King(Player::white, std::make_pair(5, 1));
    ChessPiece* whiteRook = new Rook(Player::white, std::make_pair(8, 1));
    ChessPiece* earlyMove = new Rook(Player::white, std::make_pair(1, 1));
    ChessPiece* blackKing = new King(Player::black, std::make_pair(5, 8));
    ChessPiece* blackRook = new Rook(Player::black, std::make_pair(1, 8));
    board->addPieces({whiteKing, whiteRook, earlyMove, blackKing, blackRook});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Tell the A1 king that it has already moved so white cannot castly queenside
    earlyMove->validateMove();

    // Make sure that white can castle kingside and that black can castle queenside
    CHECK(chessState->canMovePiece(std::make_pair(5, 1), std::make_pair(7, 1)));
    CHECK(chessState->canMovePiece(Player::black, std::make_pair(5, 8), std::make_pair(3, 8)));

    // Check that castling isn't possible if the rook already moved
    CHECK(!chessState->canMovePiece(std::make_pair(5, 1), std::make_pair(3, 1)));

    // Check that castling isn't possible without a rook
    CHECK(!chessState->canMovePiece(Player::black, std::make_pair(5, 8), std::make_pair(8, 8)));

    // Kingside castle with white and make sure it moved both pieces
    CHECK(chessState->movePiece(std::make_pair(5, 1), std::make_pair(7, 1)));
    CHECK(board->occupiedOnBoard(std::make_pair(6, 1)));
    CHECK(board->unoccupiedOnBoard(std::make_pair(8, 1)));

    // Queenside castle with black and make sure it moved both pieces
    CHECK(chessState->movePiece(std::make_pair(5, 8), std::make_pair(3, 8)));
    CHECK(board->occupiedOnBoard(std::make_pair(4, 8)));
    CHECK(board->unoccupiedOnBoard(std::make_pair(1, 8)));
}

TEST_CASE("King: Castling Check")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add pieces so castling would make white move past check
    ChessPiece* whiteKing  = new King(Player::white, std::make_pair(5, 1));
    ChessPiece* whiteRook1 = new Rook(Player::white, std::make_pair(8, 1));
    ChessPiece* whiteRook2 = new Rook(Player::white, std::make_pair(1, 1));
    ChessPiece* blackKing  = new King(Player::black, std::make_pair(5, 8));
    ChessPiece* blackRook1 = new Rook(Player::black, std::make_pair(4, 8));
    ChessPiece* blackRook2 = new Rook(Player::black, std::make_pair(6, 8));
    board->addPieces({whiteKing, whiteRook1, whiteRook2, blackKing, blackRook1, blackRook2});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure that white can't castle on either side because either side would
    // involve moving through check
    CHECK(!chessState->canMovePiece(std::make_pair(5, 1), std::make_pair(7, 1)));
    CHECK(!chessState->canMovePiece(Player::black, std::make_pair(5, 1), std::make_pair(3, 1)));
}
