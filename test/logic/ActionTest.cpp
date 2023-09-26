#include <vector>
#include <iostream>

#include "doctest.h"
#include "Action.h"
#include "AddPieceAction.h"
#include "TryCapturePieceAction.h"
#include "MovePieceAction.h"
#include "RemovePieceAction.h"
#include "GameBoard.h"
#include "Move.h"
#include "Piece.h"
#include "TestBoard.h"
#include "TestPieces.h"

using namespace testing;
using namespace logic;

TEST_CASE("Action: Create and Call All Action Types")
{
    // Create a GameBoard object
    GameBoard* board = new GameBoard({Player::white, Player::black});

    // Create actions of each type
    AddPieceAction add1Player          = AddPieceAction(Player::white, Piece::createNew, std::make_pair(1, 1));
    AddPieceAction add2Players         = AddPieceAction({Player::white, Player::black});
    MovePieceAction movePiece          = MovePieceAction(std::make_pair(1, 1));
    RemovePieceAction removePiece      = RemovePieceAction(std::make_pair(1, 1));
    TryCapturePieceAction capturePiece = TryCapturePieceAction(Player::black);

    // Make sure calling an add action with one player can be reversed
    CHECK(add1Player.callAction(std::make_pair(0, 0), board));
    CHECK(board->occupiedOnBoard(std::make_pair(1, 1)));
    CHECK(add1Player.reverseAction(board));
    CHECK(!board->occupiedOnBoard(std::make_pair(1, 1)));

    // Make sure calling an add action with one player works in the actual game
    CHECK(add1Player.callAction(std::make_pair(0, 0), board));
    REQUIRE(board->occupiedOnBoard(std::make_pair(1, 1)));
    Piece* piece1 = board->getPiece(std::make_pair(1, 1));
    CHECK(piece1->getPlayerAccess(Player::white));
    CHECK(!piece1->getPlayerAccess(Player::black));

    // Make sure calling an add action with multiple players can be reversed
    CHECK(add2Players.callAction(std::make_pair(0, 0), board));
    CHECK(board->occupiedOnBoard(std::make_pair(0, 0)));
    CHECK(add2Players.reverseAction(board));
    CHECK(!board->occupiedOnBoard(std::make_pair(0, 0)));

    // Make sure calling an add action with multiple players works in the actual game
    CHECK(add2Players.callAction(std::make_pair(0, 0), board));
    REQUIRE(board->occupiedOnBoard(std::make_pair(0, 0)));
    Piece* piece2 = board->getPiece(std::make_pair(0, 0));
    CHECK(piece2->getPlayerAccess(Player::white));
    CHECK(piece2->getPlayerAccess(Player::black));

    // Make sure calling a move action can be reversed
    Piece* piece3 = board->getPiece(std::make_pair(1, 1));
    piece3->unValidateMove();
    CHECK(movePiece.callAction(std::make_pair(1, 1), board));
    CHECK(!board->occupiedOnBoard(std::make_pair(1, 1)));
    CHECK(board->occupiedOnBoard(std::make_pair(2, 2)));
    CHECK(movePiece.reverseAction(board));
    CHECK(board->occupiedOnBoard(std::make_pair(1, 1)));
    CHECK(!board->occupiedOnBoard(std::make_pair(2, 2)));

    // Make sure calling a move action works in the actual game
    CHECK(movePiece.callAction(std::make_pair(1, 1), board));
    CHECK(!board->occupiedOnBoard(std::make_pair(1, 1)));
    REQUIRE(board->occupiedOnBoard(std::make_pair(2, 2)));

    // Make sure the move action's symptomatic effects work properly
    CHECK(!piece3->previouslyMoved());
    movePiece.applySymptomaticEffects(board);
    CHECK(piece3->previouslyMoved());

    // Make sure calling a remove action can be reversed
    CHECK(removePiece.callAction(std::make_pair(-1, -1), board));
    CHECK(!board->occupiedOnBoard(std::make_pair(0, 0)));
    CHECK(removePiece.reverseAction(board));
    CHECK(board->occupiedOnBoard(std::make_pair(0, 0)));

    // Make sure calling a remove action works in the actual game
    CHECK(removePiece.callAction(std::make_pair(-1, -1), board));
    CHECK(!board->occupiedOnBoard(std::make_pair(0, 0)));

    // Make sure calling a capture action can be reversed
    CHECK(capturePiece.callAction(std::make_pair(5, 5), board)); // Capture nothing and make sure nothing happens
    CHECK(capturePiece.callAction(std::make_pair(2, 2), board));
    CHECK(!board->occupiedOnBoard(std::make_pair(2, 2)));
    CHECK(capturePiece.reverseAction(board));
    CHECK(board->occupiedOnBoard(std::make_pair(2, 2)));

    // Make sure calling a capture action works in the actual game
    CHECK(capturePiece.callAction(std::make_pair(2, 2), board));
    CHECK(!board->occupiedOnBoard(std::make_pair(2, 2)));

    // Make sure the capture action's symptomattic effects work properly
    CHECK(board->getPlayerCaptures(Player::black)->size() <= 0);
    capturePiece.applySymptomaticEffects(board);
    CHECK(board->getPlayerCaptures(Player::black)->size() > 0);
}