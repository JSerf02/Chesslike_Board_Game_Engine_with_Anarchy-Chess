#include <vector>
#include <iostream>
#include <algorithm>

#include "doctest.h"
#include "TestPieces.h"

#include "ChessPiece.h"
#include "ChessBoard.h"
#include "ChessGameState.h"
#include "Move.h"
#include "Piece.h"
#include "GameBoard.h"
#include "GameState.h"

using namespace logic;
using namespace chess;
using namespace testing;
using Player = Piece::Player;

TEST_CASE("Chess Piece: Controlled By Player")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add a bishop to the board on D4 and 5 test pieces on C3, B6, H8, and G1
    ChessPiece* whitePiece = new ChessPiece(Player::white, std::make_pair(2, 2));
    ChessPiece* blackPiece = new ChessPiece(Player::black, std::make_pair(2, 5));
    board->addPieces({whitePiece, blackPiece});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState chessState = ChessGameState(board);

    CHECK(whitePiece->controlledByPlayer(chessState));
    CHECK(blackPiece->controlledByPlayer(chessState) == false);
}

TEST_CASE("Chess Piece: Add to Move")
{
    // Create a ChessGameState
    ChessGameState chessState{};

    // Make moves so moving the gPawn to G4 is checkmate and moving the gPawn to G3 is safe
    CHECK(chessState.movePiece(std::make_pair(2, 6), std::make_pair(3, 6))); // Pawn to F3
    CHECK(chessState.movePiece(std::make_pair(7, 5), std::make_pair(5, 5))); // Pawn to E5
    CHECK(chessState.movePiece(std::make_pair(2, 1), std::make_pair(3, 1))); // Pawn to A4
    CHECK(chessState.movePiece(std::make_pair(8, 4), std::make_pair(4, 8))); // Queen to H4

    // Get the current game board and get the G-Pawn from it
    GameBoard* board = chessState.getBoard();
    REQUIRE(board != nullptr);
    ChessPiece* gPawn = static_cast<ChessPiece*>(board->getPiece(2, 7));
    REQUIRE(gPawn != nullptr);

    // Create a move
    Move move{};

    // Make sure adding pawn to G4 fails
    CHECK(gPawn->addToMove(std::make_pair(4, 7), move, chessState) == false);
    CHECK(move.containsPosition(std::make_pair(3, 7)) == false);

    // Make sure adding pawn to G3 succeeds
    CHECK(gPawn->addToMove(std::make_pair(3, 7), move, chessState));
    CHECK(move.containsPosition(std::make_pair(3, 7)));
}

TEST_CASE("Chess Piece: Add Position")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add white pieces to B2 and A1 and a black piece to B5
    ChessPiece* whitePiece    = new ChessPiece(Player::white, std::make_pair(2, 2));
    ChessPiece* blackPiece    = new ChessPiece(Player::black, std::make_pair(2, 5));
    ChessPiece* blockingPiece = new ChessPiece(Player::white, std::make_pair(1, 1));
    board->addPieces({whitePiece, blackPiece, blockingPiece});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState chessState = ChessGameState(board);

    // Check if adding a legal position successfully creates a new Move
    std::vector<Move> successfulAdd = whitePiece->addPosition(std::make_pair(3, 2), chessState);
    REQUIRE(successfulAdd.size() == 1);
    CHECK(successfulAdd[0].getPositions()[0] == std::make_pair(3, 2));

    // Check if adding a legal position successfully adds to an existing Move
    CHECK(whitePiece->addPosition(std::make_pair(4, 2), successfulAdd, chessState));
    REQUIRE(successfulAdd.size() == 2);
    CHECK(successfulAdd[1].getPositions()[0] == std::make_pair(4, 2));

    // Check if adding a position occupied by an opponent's piece creates a new move
    std::vector<Move> captureAdd = whitePiece->addPosition(std::make_pair(2, 5), chessState);
    REQUIRE(successfulAdd.size() == 1);
    CHECK(successfulAdd[0].getPositions()[0] == std::make_pair(2, 5));

    // Check if adding a position occupied by an opponent's piece adds to an existing Move
    CHECK(whitePiece->addPosition(std::make_pair(2, 5), successfulAdd, chessState));
    REQUIRE(successfulAdd.size() == 3);
    CHECK(successfulAdd[2].getPositions()[0] == std::make_pair(2, 5));

    // Check if adding a position occupied by your own piece does not create a new Move
    std::vector<Move> unsuccessfulAdd = whitePiece->addPosition(std::make_pair(1, 1), chessState);
    CHECK(unsuccessfulAdd.size() == 0);

    // Check if adding a position occupied by your own piece does not add to an existing Move
    CHECK(whitePiece->addPosition(std::make_pair(1, 1), unsuccessfulAdd, chessState) == false);
    CHECK(unsuccessfulAdd.size() == 0);

    // Check if adding a legal position when it is not your turn does not create a new Move
    std::vector<Move> opponentAdd = blackPiece->addPosition(std::make_pair(5, 5), chessState);
    CHECK(opponentAdd.size() == 0);

    // Check if adding a legal position when it is not your turn does not add to an existing Move
    CHECK(blackPiece->addPosition(std::make_pair(5, 5), opponentAdd, chessState) == false);
    CHECK(opponentAdd.size() == 0);
}

TEST_CASE("Chess Piece: Add Unrelated Positions")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add white pieces to B2 and D2 and a black piece to B5
    ChessPiece* whitePiece    = new ChessPiece(Player::white, std::make_pair(2, 2));
    ChessPiece* blackPiece    = new ChessPiece(Player::black, std::make_pair(2, 5));
    ChessPiece* blockingPiece = new ChessPiece(Player::white, std::make_pair(4, 2));
    board->addPieces({whitePiece, blackPiece, blockingPiece});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 3));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState chessState = ChessGameState(board);

    // Check if adding a legal position successfully creates a new Move
    std::vector<Move::position> verticalPositions {
        std::make_pair(2, 3),
        std::make_pair(2, 4),
        std::make_pair(2, 5), // The black piece
        std::make_pair(2, 6),
        std::make_pair(2, 7),
        std::make_pair(2, 8)
    };
    std::vector<Move> verticalAdd = whitePiece->addUnrelatedPositions(verticalPositions, chessState);
    REQUIRE(verticalAdd.size() == 6); // The black piece is captured and blocking doesn't occur for unrelated positions
    for(int i = 0; i < 6; i++) {
        CHECK(verticalAdd[i].getPositions()[0] == verticalPositions[i]);
    }

    // Check if adding a legal position successfully creates a new Move
    std::vector<Move::position> horizontalPositions {
        std::make_pair(3, 2),
        std::make_pair(4, 2), // The blocking piece
        std::make_pair(5, 2), 
        std::make_pair(6, 2),
        std::make_pair(7, 2),
        std::make_pair(8, 2)
    };
    std::vector<Move> horizontalAdd{};
    whitePiece->addUnrelatedPositions(horizontalPositions, horizontalAdd, chessState);
    REQUIRE(horizontalAdd.size() == 5); // The white piece is not captured and blocking doesn't occur for unrelated positions
    for(int i = 0; i < 5; i++) {
        CHECK(horizontalAdd[i].getPositions()[0] == horizontalPositions[i + (i > 0 ? 1 : 0)]);
    }

    // Check if adding legal positions for a piece you do not control does not work
    std::vector<Move> opponentAdd = blackPiece->addUnrelatedPositions(horizontalPositions, chessState);
    CHECK(opponentAdd.size() == 0);
}

TEST_CASE("Chess Piece: Add Related Positions")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add white pieces to B2 and D2 and a black piece to B5
    ChessPiece* whitePiece    = new ChessPiece(Player::white, std::make_pair(2, 2));
    ChessPiece* blackPiece    = new ChessPiece(Player::black, std::make_pair(2, 5));
    ChessPiece* blockingPiece = new ChessPiece(Player::white, std::make_pair(4, 2));
    board->addPieces({whitePiece, blackPiece, blockingPiece});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 3));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState chessState = ChessGameState(board);

    // Check if adding a legal position successfully creates a new Move
    std::vector<Move::position> verticalPositions {
        std::make_pair(2, 3),
        std::make_pair(2, 4),
        std::make_pair(2, 5), // The black piece
        std::make_pair(2, 6),
        std::make_pair(2, 7),
        std::make_pair(2, 8)
    };
    std::vector<Move> verticalAdd = whitePiece->addRelatedPositions(verticalPositions, chessState);
    REQUIRE(verticalAdd.size() == 1); // All positions are added to the same move
    REQUIRE(verticalAdd[0].getPositions().size() == 3); // The black piece is captured and blocking does occur
    for(int i = 0; i < 3; i++) {
        CHECK(verticalAdd[0].getPositions()[i] == verticalPositions[i]);
    }

    // Check if adding a legal position successfully creates a new Move
    std::vector<Move::position> horizontalPositions {
        std::make_pair(3, 2),
        std::make_pair(4, 2), // The blocking piece
        std::make_pair(5, 2), 
        std::make_pair(6, 2),
        std::make_pair(7, 2),
        std::make_pair(8, 2)
    };
    std::vector<Move> horizontalAdd{};
    whitePiece->addRelatedPositions(horizontalPositions, horizontalAdd, chessState);
    REQUIRE(horizontalAdd.size() == 1); // All positions are added to the same move
    REQUIRE(horizontalAdd[0].getPositions().size() == 1); // The white piece is not captured and blocking does occur
    CHECK(horizontalAdd[0].getPositions()[0] == horizontalPositions[0]);

    // Check if adding legal positions for a piece you do not control does not work
    std::vector<Move> opponentAdd = blackPiece->addRelatedPositions(horizontalPositions, chessState);
    CHECK(opponentAdd.size() == 0);
}

TEST_CASE("Chess Piece: Add Unrelated Positions With Deltas")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add white pieces to B2 and D2 and a black piece to B5
    ChessPiece* whitePiece    = new ChessPiece(Player::white, std::make_pair(2, 2));
    ChessPiece* blackPiece    = new ChessPiece(Player::black, std::make_pair(2, 5));
    ChessPiece* blockingPiece = new ChessPiece(Player::white, std::make_pair(4, 2));
    board->addPieces({whitePiece, blackPiece, blockingPiece});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 3));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState chessState = ChessGameState(board);

    // Check if adding a legal position successfully creates a new Move
    std::vector<Move::position> verticalPositions {
        std::make_pair(2, 3),
        std::make_pair(2, 4),
        std::make_pair(2, 5), // The black piece
        std::make_pair(2, 6),
        std::make_pair(2, 7),
        std::make_pair(2, 8)
    };
    std::vector<Move::position> verticalDeltas {
        std::make_pair(0, 1),
        std::make_pair(0, 2),
        std::make_pair(0, 3), // The black piece
        std::make_pair(0, 4),
        std::make_pair(0, 5),
        std::make_pair(0, 6)
    };
    std::vector<Move> verticalAdd = whitePiece->addUnrelatedPositionsDeltas(verticalDeltas, chessState);
    REQUIRE(verticalAdd.size() == 6); // The black piece is captured and blocking doesn't occur for unrelated positions
    for(int i = 0; i < 6; i++) {
        CHECK(verticalAdd[i].getPositions()[0] == verticalPositions[i]);
    }

    // Check if adding a legal position successfully creates a new Move
    std::vector<Move::position> horizontalPositions {
        std::make_pair(3, 2),
        std::make_pair(4, 2), // The blocking piece
        std::make_pair(5, 2), 
        std::make_pair(6, 2),
        std::make_pair(7, 2),
        std::make_pair(8, 2)
    };
    std::vector<Move::position> horizontalDeltas {
        std::make_pair(1, 0),
        std::make_pair(2, 0), // The blocking piece
        std::make_pair(3, 0),
        std::make_pair(4, 0),
        std::make_pair(5, 0),
        std::make_pair(6, 0),
    };
    std::vector<Move> horizontalAdd{};
    whitePiece->addUnrelatedPositionsDeltas(horizontalDeltas, horizontalAdd, chessState);
    REQUIRE(horizontalAdd.size() == 5); // The white piece is not captured and blocking doesn't occur for unrelated positions
    for(int i = 0; i < 5; i++) {
        CHECK(horizontalAdd[i].getPositions()[0] == horizontalPositions[i + (i > 0 ? 1 : 0)]);
    }

    // Check if adding legal positions for a piece you do not control does not work
    std::vector<Move> opponentAdd = blackPiece->addUnrelatedPositionsDeltas(horizontalDeltas, chessState);
    CHECK(opponentAdd.size() == 0);
}

TEST_CASE("Chess Piece: Add Related Positions")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add white pieces to B2 and D2 and a black piece to B5
    ChessPiece* whitePiece    = new ChessPiece(Player::white, std::make_pair(2, 2));
    ChessPiece* blackPiece    = new ChessPiece(Player::black, std::make_pair(2, 5));
    ChessPiece* blockingPiece = new ChessPiece(Player::white, std::make_pair(4, 2));
    board->addPieces({whitePiece, blackPiece, blockingPiece});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 3));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState chessState = ChessGameState(board);

    // Check if adding a legal position successfully creates a new Move
    std::vector<Move::position> verticalPositions {
        std::make_pair(2, 3),
        std::make_pair(2, 4),
        std::make_pair(2, 5), // The black piece
        std::make_pair(2, 6),
        std::make_pair(2, 7),
        std::make_pair(2, 8)
    };
    std::vector<Move::position> verticalDeltas {
        std::make_pair(0, 1),
        std::make_pair(0, 2),
        std::make_pair(0, 3), // The black piece
        std::make_pair(0, 4),
        std::make_pair(0, 5),
        std::make_pair(0, 6)
    };
    std::vector<Move> verticalAdd = whitePiece->addRelatedPositionsDeltas(verticalDeltas, chessState);
    REQUIRE(verticalAdd.size() == 1); // All positions are added to the same move
    REQUIRE(verticalAdd[0].getPositions().size() == 3); // The black piece is captured and blocking does occur
    for(int i = 0; i < 3; i++) {
        CHECK(verticalAdd[0].getPositions()[i] == verticalPositions[i]);
    }

    // Check if adding a legal position successfully creates a new Move
    std::vector<Move::position> horizontalPositions {
        std::make_pair(3, 2),
        std::make_pair(4, 2), // The blocking piece
        std::make_pair(5, 2), 
        std::make_pair(6, 2),
        std::make_pair(7, 2),
        std::make_pair(8, 2)
    };
    std::vector<Move::position> horizontalDeltas {
        std::make_pair(1, 0),
        std::make_pair(2, 0), // The blocking piece
        std::make_pair(3, 0),
        std::make_pair(4, 0),
        std::make_pair(5, 0),
        std::make_pair(6, 0),
    };
    std::vector<Move> horizontalAdd{};
    whitePiece->addRelatedPositionsDeltas(horizontalDeltas, horizontalAdd, chessState);
    REQUIRE(horizontalAdd.size() == 1); // All positions are added to the same move
    REQUIRE(horizontalAdd[0].getPositions().size() == 1); // The white piece is not captured and blocking does occur
    CHECK(horizontalAdd[0].getPositions()[0] == horizontalPositions[0]);

    // Check if adding legal positions for a piece you do not control does not work
    std::vector<Move> opponentAdd = blackPiece->addRelatedPositionsDeltas(horizontalDeltas, chessState);
    CHECK(opponentAdd.size() == 0);
}