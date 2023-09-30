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
#include "Pawn.h"
#include "Rook.h"
#include "King.h"

using namespace logic;
using namespace chess;
using namespace testing;

using Player = Piece::Player;

TEST_CASE("Pawn: Regular Movement and Boost")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add pawns to B2 and D7
    ChessPiece* whitePawn = new Pawn(Player::white, std::make_pair(2, 2));
    ChessPiece* blackPawn = new Pawn(Player::black, std::make_pair(4, 7));
    board->addPieces({whitePawn, blackPawn});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure the white pawn can only move to valid positions
    // Valid positions in this case are the regular and boost positions
    std::set<Move::position> validPositions1 {
        std::make_pair(2, 3), std::make_pair(2, 4)
    };
    TestChessHelpers::testPiecePositions(chessState, whitePawn->getPosition(), validPositions1);

    // Make sure the pawn can move forwards but cannot promote
    CHECK(chessState->getMovesOfPiece(std::make_pair(2, 2), std::make_pair(2, 3)).size() == 1);

    // Move the pawn without boosting
    CHECK(chessState->movePiece(std::make_pair(2, 2), std::make_pair(2, 3)));

    // Make sure the black pawn can only move to valid positions
    // Valid positions in this case are the regular and boost positions
    std::set<Move::position> validPositions2 {
        std::make_pair(4, 6), std::make_pair(4, 5)
    };
    TestChessHelpers::testPiecePositions(chessState, blackPawn->getPosition(), validPositions2);

    // Move the black pawn with a boost
    CHECK(chessState->movePiece(std::make_pair(4, 7), std::make_pair(4, 5)));

    // Make sure the white pawn can only move to valid positions
    // Valid position is one ahead of the pawn since boosting is no longer possible
    std::set<Move::position> validPositions3 {
        std::make_pair(2, 4)
    };
    TestChessHelpers::testPiecePositions(chessState, whitePawn->getPosition(), validPositions3);

    // Move the white pawn
    CHECK(chessState->movePiece(std::make_pair(2, 3), std::make_pair(2, 4)));

    // Make sure the black pawn can only move to valid positions
    // Valid position is one ahead of the pawn since boosting is no longer possible
    std::set<Move::position> validPositions4 {
        std::make_pair(4, 4)
    };
    TestChessHelpers::testPiecePositions(chessState, blackPawn->getPosition(), validPositions4);
}

TEST_CASE("Pawn: Blocked Movement")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add pawns to B2 and C7 and add test pieces to B4 and C6
    ChessPiece* whitePawn  = new Pawn(          Player::white, std::make_pair(2, 2));
    ChessPiece* blackPawn  = new Pawn(          Player::black, std::make_pair(3, 7));
    ChessPiece* whiteBlock = new TestChessPiece(Player::white, std::make_pair(2, 4));
    ChessPiece* blackBlock = new TestChessPiece(Player::black, std::make_pair(3, 6));
    board->addPieces({whitePawn, blackPawn, whiteBlock, blackBlock});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure the white pawn can only move to valid positions
    // Valid position is one ahead of the pawn since the boost position is blocked
    std::set<Move::position> validPositions1 {
        std::make_pair(2, 3)
    };
    TestChessHelpers::testPiecePositions(chessState, whitePawn->getPosition(), validPositions1);

    // Move the white pawn
    CHECK(chessState->movePiece(std::make_pair(2, 2), std::make_pair(2, 3)));

    // Make sure the black pawn cannot move because it is blocked
    std::set<Move::position> validPositions2{};
    TestChessHelpers::testPiecePositions(chessState, blackPawn->getPosition(), validPositions2);
}

TEST_CASE("Pawn: Attack")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add pawns to B2 and C7 and add a test pieces to B4
    ChessPiece* whitePawn  = new Pawn(          Player::white, std::make_pair(2, 2));
    ChessPiece* blackPawn  = new Pawn(          Player::black, std::make_pair(3, 7));
    board->addPieces({whitePawn, blackPawn});

    // Add enemy pieces to A3, C3, B6, and D6
    ChessPiece* blackEnemy1 = new TestChessPiece(Player::black, std::make_pair(1, 3));
    ChessPiece* blackEnemy2 = new TestChessPiece(Player::black, std::make_pair(3, 3));
    ChessPiece* whiteEnemy1 = new TestChessPiece(Player::white, std::make_pair(2, 6));
    ChessPiece* whiteEnemy2 = new TestChessPiece(Player::white, std::make_pair(4, 6));
    board->addPieces({blackEnemy1, blackEnemy2, whiteEnemy1, whiteEnemy2});

    // Add blocking pieces to B4 and C4
    ChessPiece* level2Block1 = new TestChessPiece(Player::white, std::make_pair(2, 4));
    ChessPiece* level2Block2 = new TestChessPiece(Player::white, std::make_pair(3, 4));
    board->addPieces({level2Block1, level2Block2});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 2));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure the white pawn can only move to valid positions
    // Valid positions are the 2 diagonal attack positions 
    std::set<Move::position> validPositions1 {
        std::make_pair(1, 3),
        std::make_pair(2, 3),
        std::make_pair(3, 3)
    };
    TestChessHelpers::testPiecePositions(chessState, whitePawn->getPosition(), validPositions1);

    // Make sure the white pawn is only attacking valid positions
    // Valid positions are the 2 diagonals ahead of the pawn
    std::set<Move::position> validAttacks1 {
        std::make_pair(1, 3),
        std::make_pair(3, 3)
    };
    TestChessHelpers::testPieceAttacks(chessState, Player::black, validAttacks1);

    // Capture the piece at C3
    CHECK(chessState->movePiece(std::make_pair(2, 2), std::make_pair(3, 3)));

    // Make sure the black pawn can only move to valid positions
    // Valid positions are one ahead of the pawn, 2 ahead of the pawn,
    // and the 2 diagonal attack positions
    std::set<Move::position> validPositions2 {
        std::make_pair(3, 6),
        std::make_pair(3, 5),
        std::make_pair(2, 6),
        std::make_pair(4, 6)
    };
    TestChessHelpers::testPiecePositions(chessState, blackPawn->getPosition(), validPositions2);

    // Make sure the black pawn is only attacking valid positions
    // Valid positions are the 2 diagonals ahead of the pawn
    std::set<Move::position> validAttacks2 {
        std::make_pair(2, 6),
        std::make_pair(4, 6)
    };
    TestChessHelpers::testPieceAttacks(chessState, Player::white, validAttacks2);

    // Capture the piece at B6
    CHECK(chessState->movePiece(std::make_pair(3, 7), std::make_pair(2, 6)));

    // Make sure pieces cannot attack themselves or the air
    std::set<Move::position> validPositions3 {}; // White pawn cannot move
    TestChessHelpers::testPiecePositions(chessState, whitePawn->getPosition(), validPositions3); 
    std::set<Move::position> validAttacks3 {
        std::make_pair(4, 4)
    };
    TestChessHelpers::testPieceAttacks(chessState, Player::black, validAttacks3);
}

TEST_CASE("Pawn: Value")
{
    // Create a ChessBoard
    ChessBoard* board = new ChessBoard();

    // Get a pawn from the chess board
    Piece* pawn = board->getPiece(1, 2);

    // Make sure the pawn's value is 1.0
    CHECK(pawn->getValue() == 1.0);

    // Make sure the pawn's ID is correctly set
    CHECK(pawn->getID() == PAWN_ID);
}

TEST_CASE("Pawn: En Passant")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add pieces to create this board state:
    // https://lichess.org/editor/8/3p4/8/2P1P1p1/p1p5/8/1P5P/6Kk_w_-_-_0_1?color=white
    ChessPiece* whiteBoost1      = new Pawn(Player::white, std::make_pair(2, 2));
    ChessPiece* blackEnPassant1  = new Pawn(Player::black, std::make_pair(1, 4));
    ChessPiece* blackEnPassant2  = new Pawn(Player::black, std::make_pair(3, 4));
    ChessPiece* blackBoost       = new Pawn(Player::black, std::make_pair(4, 7));
    ChessPiece* whiteEnPassant1  = new Pawn(Player::white, std::make_pair(3, 5));
    ChessPiece* whiteEnPassant2  = new Pawn(Player::white, std::make_pair(5, 5));
    ChessPiece* whiteBoost2      = new Pawn(Player::white, std::make_pair(8, 2));
    ChessPiece* blackFailPassant = new Pawn(Player::black, std::make_pair(7, 5));
    board->addPieces({
        whiteBoost1, blackEnPassant1, blackEnPassant2,
        blackBoost,  whiteEnPassant1, whiteEnPassant2,
        whiteBoost2, blackFailPassant
    });

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(7, 8));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure the black pieces cannot already En Passant
    std::set<Move::position> validPositions1 {
        std::make_pair(1, 3)
    };
    TestChessHelpers::testPiecePositions(chessState, blackEnPassant1->getPosition(), validPositions1);
    std::set<Move::position> validPositions2 {
        std::make_pair(3, 3)
    };
    TestChessHelpers::testPiecePositions(chessState, blackEnPassant2->getPosition(), validPositions2);
    
    // Boost the pawn on B2
    CHECK(chessState->movePiece(std::make_pair(2, 2), std::make_pair(2, 4)));

    // Make sure the black pieces can now En Passant
    // Also make sure that the black pieces cannot do anything else since En Passant
    // is forced
    std::set<Move::position> validPositions3 {
        std::make_pair(2, 3)
    };
    TestChessHelpers::testPiecePositions(chessState, blackEnPassant1->getPosition(), validPositions3);
    TestChessHelpers::testPiecePositions(chessState, blackEnPassant2->getPosition(), validPositions3);

    // En Passant and make sure the piece was captured
    CHECK(chessState->movePiece(std::make_pair(1, 4), std::make_pair(2, 3)));
    CHECK(board->unoccupiedOnBoard(2, 4));

    // Make sure the white pieces cannot already En Passant
    std::set<Move::position> validPositions4 {
        std::make_pair(3, 6)
    };
    TestChessHelpers::testPiecePositions(chessState, whiteEnPassant1->getPosition(), validPositions4);
    std::set<Move::position> validPositions5 {
        std::make_pair(5, 6)
    };
    TestChessHelpers::testPiecePositions(chessState, whiteEnPassant2->getPosition(), validPositions5);

    // Boost the pawn on H2 
    CHECK(chessState->movePiece(std::make_pair(8, 2), std::make_pair(8, 4)));

    // Boost the pawn on D7
    CHECK(chessState->movePiece(std::make_pair(4, 7), std::make_pair(4, 5)));

    // Make sure the white pieces can now En Passant
    // Also make sure that the white pieces cannot do anything else since En Passant
    // is forced
    std::set<Move::position> validPositions6 {
        std::make_pair(4, 6)
    };
    TestChessHelpers::testPiecePositions(chessState, whiteEnPassant1->getPosition(), validPositions6);
    TestChessHelpers::testPiecePositions(chessState, whiteEnPassant2->getPosition(), validPositions6);

    // En Passant and make sure the piece was captured
    CHECK(chessState->movePiece(std::make_pair(5, 5), std::make_pair(4, 6)));
    CHECK(board->unoccupiedOnBoard(4, 5));

    // Move the pawn from G5 to G4
    CHECK(chessState->movePiece(std::make_pair(7, 5), std::make_pair(7, 4)));

    // Random move to stall so black can move again
    CHECK(chessState->movePiece(std::make_pair(4, 6), std::make_pair(4, 7)));

    // Make sure black cannot En Passant with the G pawn since a turn has passed 
    // since the H pawn's boost
    std::set<Move::position> validPositions7 {
        std::make_pair(7, 3)
    };
    TestChessHelpers::testPiecePositions(chessState, blackFailPassant->getPosition(), validPositions7);
}

// Using regular chess pieces for this test to make it an integration test
TEST_CASE("Pawn: En Passant Min Priority Override")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add pieces to create this board state:
    // https://lichess.org/editor/k7/8/8/8/p7/8/1P6/R6K_w_-_-_0_1?color=white
    ChessPiece* whiteBoost     = new Pawn(Player::white, std::make_pair(2, 2));
    ChessPiece* blackEnPassant = new Pawn(Player::black, std::make_pair(1, 4));
    ChessPiece* whiteAttacker  = new Rook(Player::white, std::make_pair(1, 1));
    ChessPiece* whiteKing = new King(Player::white, std::make_pair(8, 1));
    ChessPiece* blackKing = new King(Player::black, std::make_pair(1, 8));
    board->addPieces({
        whiteBoost, blackEnPassant, whiteAttacker, whiteKing, blackKing
    });

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure black has valid moves
    CHECK(chessState->canMove(Player::black));

    // Boost the white pawn to make En Passant possible
    CHECK(chessState->movePiece(std::make_pair(2, 2), std::make_pair(2, 4)));

    // Make sure black can't move since En Passant is impossible (it would put
    // black into check) but it still applies the priority override
    CHECK(!chessState->canMove(Player::black));
}

TEST_CASE("Pawn: Promotion")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add 2 pawns of each color near the edges of the board
    ChessPiece* queenPromote  = new Pawn(Player::white, std::make_pair(1, 7));
    ChessPiece* knightPromote = new Pawn(Player::black, std::make_pair(3, 2));
    ChessPiece* rookPromote   = new Pawn(Player::white, std::make_pair(5, 7));
    ChessPiece* bishopPromote = new Pawn(Player::black, std::make_pair(7, 2));
    ChessPiece* knookPromote  = new Pawn(Player::white, std::make_pair(2, 7));
    board->addPieces({
        queenPromote, knightPromote, rookPromote, bishopPromote, knookPromote
    });

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 4));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 5));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Promote to a queen
    REQUIRE(chessState->movePiece(std::make_pair(1, 7), std::make_pair(1, 8), static_cast<int>(Pawn::PromotionIdx::queen)));
    CHECK(board->getPiece(std::make_pair(1, 8))->getID() == QUEEN_ID);

    // Promote to a knight
    REQUIRE(chessState->movePiece(std::make_pair(3, 2), std::make_pair(3, 1), static_cast<int>(Pawn::PromotionIdx::knight)));
    CHECK(board->getPiece(std::make_pair(3, 1))->getID() == KNIGHT_ID);

    // Promote to a rook
    REQUIRE(chessState->movePiece(std::make_pair(5, 7), std::make_pair(5, 8), static_cast<int>(Pawn::PromotionIdx::rook)));
    CHECK(board->getPiece(std::make_pair(5, 8))->getID() == ROOK_ID);

    // Promote to a bishop
    REQUIRE(chessState->movePiece(std::make_pair(7, 2), std::make_pair(7, 1), static_cast<int>(Pawn::PromotionIdx::bishop)));
    CHECK(board->getPiece(std::make_pair(7, 1))->getID() == BISHOP_ID);

    // Promote to a knook
    REQUIRE(chessState->movePiece(std::make_pair(2, 7), std::make_pair(2, 8), static_cast<int>(Pawn::PromotionIdx::knook)));
    CHECK(board->getPiece(std::make_pair(2, 8))->getID() == KNOOK_ID);
}

TEST_CASE("Pawn: Knight Boosting")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add 2 pawns near the edge of the board
    ChessPiece* whitePawn = new Pawn(Player::white, std::make_pair(4, 7));
    ChessPiece* blackPawn = new Pawn(Player::black, std::make_pair(4, 2));
    board->addPieces({whitePawn, blackPawn});

    // Add 2 kings to the board to prevent errors
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 8));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 7));
    board->addPieces({whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure all possible boost positions are present
    CHECK(chessState->canMovePiece(whitePawn->getPosition(), std::make_pair(2, 7)));
    CHECK(chessState->canMovePiece(whitePawn->getPosition(), std::make_pair(3, 6)));
    CHECK(chessState->canMovePiece(whitePawn->getPosition(), std::make_pair(5, 6)));
    CHECK(chessState->canMovePiece(whitePawn->getPosition(), std::make_pair(6, 7)));

    // Make sure all possible boost positions are attacked
    CHECK(chessState->isAttacked(Player::black, std::make_pair(2, 7)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(3, 6)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(5, 6)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(6, 7)));

    // Move the piece and make sure it was promoted into a knight
    CHECK(chessState->movePiece(whitePawn->getPosition(), std::make_pair(5, 6)));
    Piece* piece = board->getPiece(std::make_pair(5, 6));
    REQUIRE(piece != nullptr);
    CHECK(piece->getID() == KNIGHT_ID);

    // Make sure all possible boost positions are present
    CHECK(chessState->canMovePiece(blackPawn->getPosition(), std::make_pair(2, 2)));
    CHECK(chessState->canMovePiece(blackPawn->getPosition(), std::make_pair(3, 3)));
    CHECK(chessState->canMovePiece(blackPawn->getPosition(), std::make_pair(5, 3)));
    CHECK(chessState->canMovePiece(blackPawn->getPosition(), std::make_pair(6, 2)));

    // Make sure all possible boost positions are attacked
    CHECK(chessState->isAttacked(Player::white, std::make_pair(2, 2)));
    CHECK(chessState->isAttacked(Player::white, std::make_pair(3, 3)));
    CHECK(chessState->isAttacked(Player::white, std::make_pair(5, 3)));
    CHECK(chessState->isAttacked(Player::white, std::make_pair(6, 2)));

    // Move the piece and make sure it was promoted into a knight
    CHECK(chessState->movePiece(blackPawn->getPosition(), std::make_pair(2, 2)));
    Piece* otherPiece = board->getPiece(std::make_pair(2, 2));
    REQUIRE(otherPiece != nullptr);
    CHECK(otherPiece->getID() == KNIGHT_ID);
}

TEST_CASE("Pawn: Super Passant")
{
    // Create a ChessBoard without any pieces
    ChessBoard* board = new ChessBoard(false);

    // Add pieces to setup this board state
    // https://lichess.org/editor/8/2p1p3/3p4/5P2/8/8/7K/7k_w_-_-_0_1?color=white
    ChessPiece* whitePawn  = new Pawn(Player::white, std::make_pair(6, 5));
    ChessPiece* blackPawn1 = new Pawn(Player::black, std::make_pair(5, 7));
    ChessPiece* blackPawn2 = new Pawn(Player::black, std::make_pair(4, 6));
    ChessPiece* blackPawn3 = new Pawn(Player::black, std::make_pair(3, 7));
    ChessPiece* whiteKing = new TestKing(Player::white, std::make_pair(8, 2));
    ChessPiece* blackKing = new TestKing(Player::black, std::make_pair(8, 1));
    board->addPieces({whitePawn, blackPawn1, blackPawn2, blackPawn3, whiteKing, blackKing});

    // Create a ChessGameState
    ChessGameState* chessState = new ChessGameState(board);

    // Make sure the pawn cannot move before the enemy pawn boosts
    CHECK(!chessState->canMovePiece(std::make_pair(6, 5), std::make_pair(5, 6)));

    // Boost the E7 black pawn
    CHECK(chessState->setNextPlayer());
    CHECK(chessState->movePiece(std::make_pair(5, 7), std::make_pair(5, 5)));

    // Make sure the pawn can chain En Passants
    CHECK(chessState->canMovePiece(std::make_pair(6, 5), std::make_pair(5, 6)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(5, 5)));
    CHECK(chessState->canMovePiece(std::make_pair(6, 5), std::make_pair(4, 7)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(4, 6)));
    CHECK(chessState->canMovePiece(std::make_pair(6, 5), std::make_pair(3, 8)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(3, 7)));

    // Make sure the pawn can promote to every possible piece
    CHECK(chessState->getMovesOfPiece(std::make_pair(6, 5), std::make_pair(3, 8)).size() == static_cast<int>(Pawn::PromotionIdx::length));

    // Make sure the pawn can knight boost after promotion
    CHECK(chessState->canMovePiece(std::make_pair(6, 5), std::make_pair(1, 7)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(1, 7)));
    CHECK(chessState->canMovePiece(std::make_pair(6, 5), std::make_pair(2, 6)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(2, 6)));
    CHECK(chessState->canMovePiece(std::make_pair(6, 5), std::make_pair(4, 6)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(4, 6)));
    CHECK(chessState->canMovePiece(std::make_pair(6, 5), std::make_pair(5, 7)));
    CHECK(chessState->isAttacked(Player::black, std::make_pair(5, 7)));

    // Move the pawn to E7 (long passant -> knight boost)
    CHECK(chessState->movePiece(std::make_pair(6, 5), std::make_pair(5, 7)));

    // Make sure the enemy pawns were all captured
    CHECK(board->unoccupiedOnBoard(std::make_pair(3, 7)));
    CHECK(board->unoccupiedOnBoard(std::make_pair(4, 6)));
    CHECK(board->unoccupiedOnBoard(std::make_pair(5, 5)));
    
    // Make sure the pawn promoted into a knight
    Piece* knight = board->getPiece(std::make_pair(5, 7));
    REQUIRE(knight != nullptr);
    CHECK(knight->getID() == KNIGHT_ID);
}
