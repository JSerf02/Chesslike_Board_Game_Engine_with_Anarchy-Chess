#include <vector>
#include <iostream>
#include <algorithm>

#include "doctest.h"
#include "Move.h"
#include "Piece.h"
#include "GameBoard.h"
#include "TestBoard.h"
#include "TestPieces.h"
#include "GameState.h"

using namespace testing;
using namespace logic;

using Player = Piece::Player;

TEST_CASE("Game State: Initialize and Get Board") 
{
    // Create an xBoard to distinguish from the default board
    PositiveXBoard* xBoard = new PositiveXBoard{};

    // Create a GameState with the xBoard
    GameState gameState{ xBoard };

    // Retrieve gameState's internal board and make sure something was returned
    GameBoard* gameStateBoard = gameState.getBoard();
    REQUIRE(gameStateBoard != nullptr);

    // Make sure the board retrieved was properly set to xBoard
    CHECK(gameStateBoard->onBoard(0, 0));
    CHECK(gameStateBoard->onBoard(-1, 0) == false);
}

TEST_CASE("Game State: Get player - default initialization")
{
    // Create a default GameState
    GameState gameState{};

    // Make sure the default player is set to white and that retrieving it functions correctly
    CHECK(gameState.getPlayer(0) == Player::white);
    CHECK(gameState.getCrntPlayer() == Player::white);

}

TEST_CASE("Game State: Get player - direct player assignment")
{
    // Create a GameState with 3 players: black, gold, and silver
    std::vector<Player> players = { Player::black, Player::gold, Player::silver };
    GameState gameState{nullptr, players};

    // Make sure the first player is black, the second gold, and the third silver
    CHECK(gameState.getPlayer(0) == Player::black);
    CHECK(gameState.getCrntPlayer() == Player::black);
    CHECK(gameState.getPlayer(1) == Player::gold);
    CHECK(gameState.getPlayer(2) == Player::silver);
}

TEST_CASE("Game State: Get player - edge cases")
{
    // Create a GameState with 0 players
    std::vector<Player> noPlayers{};
    GameState gameState{nullptr, noPlayers};

    // Make sure last is returned when trying to get player
    CHECK(gameState.getPlayer(0) == Player::last);
    CHECK(gameState.getCrntPlayer() == Player::last);

    // Make sure attempts to change the player fail
    CHECK(gameState.setCrntPlayer(0) == false);
    CHECK(gameState.setCrntPlayer(1) == false);
    CHECK(gameState.setNextPlayer() == false);
}

TEST_CASE("Game State: Set current and next players")
{
    // Create a GameState with 3 players: black, gold, and silver
    std::vector<Player> players = { Player::black, Player::gold, Player::silver };
    GameState gameState{ nullptr, players };

    // Move 1 player after the current player and ensure values update properly
    CHECK(gameState.setCrntPlayer(1));
    CHECK(gameState.getCrntPlayer() == Player::gold);
    CHECK(gameState.getPlayer(1) == Player::silver);

    // Move 2 players after the current player, causing overlap back to black
    CHECK(gameState.setCrntPlayer(2));
    CHECK(gameState.getCrntPlayer() == Player::black);

    // See if setNextPlayer() works
    CHECK(gameState.setNextPlayer());
    CHECK(gameState.getCrntPlayer() == Player::gold);
}

TEST_CASE("Game State: Get pieces controlled by current player")
{
    // Create a gameBoard
    GameBoard* gameBoard = new GameBoard{{ Player::white, Player::black }};

    // Add 3 white pieces and 2 black piecces to the board
    Piece* whitePiece1 = new Piece{Player::white};
    Piece* whitePiece2 = new Piece{Player::white, std::make_pair(1, 0)};
    Piece* whitePiece3 = new Piece{Player::white, std::make_pair(2, 0)};
    Piece* blackPiece1 = new Piece{Player::black, std::make_pair(0, 1)};
    Piece* blackPiece2 = new Piece{Player::black, std::make_pair(0, 2)};
    CHECK(gameBoard->addPieces({ whitePiece1, whitePiece2, whitePiece3, blackPiece1, blackPiece2 }));

    // Create a GameState with 2 players: white and black
    GameState gameState{ gameBoard, { Player::white, Player::black } };

    // Make sure the current player is white
    CHECK(gameState.getCrntPlayer() == Player::white);

    // Get the pieces of the current player
    std::vector<Move::position> crntPlayerPieces = gameState.getPiecesOfCrntPlayer();

    // Make sure pieces were properly returned
    for(int i = 0; i <=2; i++) { // (0, 0), (1, 0), and (2, 0) are in crntPlayerPieces
        CHECK(std::find(crntPlayerPieces.begin(), crntPlayerPieces.end(), std::make_pair(i, 0)) != crntPlayerPieces.end());
    }
    for(int j = 1; j <= 2; j++) { // (0, 1), (0, 2) are not in crntPlayerPieces
        CHECK(std::find(crntPlayerPieces.begin(), crntPlayerPieces.end(), std::make_pair(0, j)) == crntPlayerPieces.end());
    }
}

TEST_CASE("Game State: Get maximum priority of player's moves")
{
    // Create a gameBoard
    GameBoard* gameBoard = new GameBoard{{ Player::white, Player::black }};

    // Add 3 white pieces and 2 black piecces to the board with increasing priorities
    Priority1Piece* whitePiece1 = new Priority1Piece{ Player::white };
    Priority2Piece* whitePiece2 = new Priority2Piece{ Player::white, std::make_pair(1, 0) };
    Priority3Piece* whitePiece3 = new Priority3Piece{ Player::white, std::make_pair(2, 0) };
    Priority4Piece* blackPiece4 = new Priority4Piece{ Player::black, std::make_pair(0, 1) };
    Priority5Piece* blackPiece5 = new Priority5Piece{ Player::black, std::make_pair(0, 2) };
    CHECK(gameBoard->addPieces({ whitePiece1, whitePiece2, whitePiece3, blackPiece4, blackPiece5 }));

    // Create a GameState with 2 players: white and black
    GameState gameState{ gameBoard, { Player::white, Player::black } };

    CHECK(gameState.getPriorityOfPlayer(Player::white) == 3);
    CHECK(gameState.getPriority() == 3);
    CHECK(gameState.getPriorityOfPlayer(Player::black) == 5);
}

TEST_CASE("Game State: Get moves of piece")
{
    // Create a gameBoard
    GameBoard* gameBoard = new GameBoard{{ Player::white, Player::black }};

    // Create 3 testing pieces and add them to the board
    FiveFivesOneOnePiece* manyPiece = new FiveFivesOneOnePiece{ Player::white }; // 5 5-priority moves to (1, 1) and 1 1-priority move to (1, 1)
    Priority3Piece* white3Piece = new Priority3Piece{ Player::white, std::make_pair(1, 0) };
    Priority3Piece* black3Piece = new Priority3Piece{ Player::black, std::make_pair(2, 0) };
    CHECK(gameBoard->addPieces({ manyPiece, white3Piece, black3Piece }));

    // Create a GameState with 2 players: white and black
    GameState gameState{ gameBoard, { Player::white, Player::black } };
    CHECK(gameState.getCrntPlayer() == Player::white);

    // Make sure 5 out of the 6 moves of manyPiece are returned (all but the priority 1 move)
    CHECK(gameState.getMovesOfPiece(manyPiece->getPosition(), std::make_pair(1, 1)).size() == 5);
    CHECK(gameState.getMovesOfPiece(0, 0, std::make_pair(1, 1)).size() == 5);
    CHECK(gameState.getMovesOfPiece(manyPiece->getPosition(), 1, 1).size() == 5);
    CHECK(gameState.getMovesOfPiece(0, 0, 1, 1).size() == 5);

    // Make sure the move of white3Piece is not returned because its priority is too low
    CHECK(gameState.getMovesOfPiece(white3Piece->getPosition(), std::make_pair(3, 3)).size() == 0);
    CHECK(gameState.getMovesOfPiece(1, 0, std::make_pair(3, 3)).size() == 0);
    CHECK(gameState.getMovesOfPiece(white3Piece->getPosition(), 3, 3).size() == 0);
    CHECK(gameState.getMovesOfPiece(1, 0, 3, 3).size() == 0);

    // Make sure black3Piece returns no moves because white doesn't control it
    CHECK(gameState.getMovesOfPiece(black3Piece->getPosition(), std::make_pair(3, 3)).size() == 0);
    CHECK(gameState.getMovesOfPiece(2, 0, std::make_pair(3, 3)).size() == 0);
    CHECK(gameState.getMovesOfPiece(black3Piece->getPosition(), 3, 3).size() == 0);
    CHECK(gameState.getMovesOfPiece(2, 0, 3, 3).size() == 0);

    // Make sure black3Piece returns its move when checked for moves black has for that piece
    CHECK(gameState.getMovesOfPiece(Player::black, black3Piece->getPosition(), std::make_pair(3, 3)).size() == 1);
    CHECK(gameState.getMovesOfPiece(Player::black, 2, 0, std::make_pair(3, 3)).size() == 1);
    CHECK(gameState.getMovesOfPiece(Player::black, black3Piece->getPosition(), 3, 3).size() == 1);
    CHECK(gameState.getMovesOfPiece(Player::black, 2, 0, 3, 3).size() == 1);

    // Make sure explicitly checking for white's moves doesn't change anything
    CHECK(gameState.getMovesOfPiece(Player::white, black3Piece->getPosition(), std::make_pair(3, 3)).size() == 0);
    CHECK(gameState.getMovesOfPiece(Player::white, 2, 0, std::make_pair(3, 3)).size() == 0);
    CHECK(gameState.getMovesOfPiece(Player::white, black3Piece->getPosition(), 3, 3).size() == 0);
    CHECK(gameState.getMovesOfPiece(Player::white, 2, 0, 3, 3).size() == 0);
}


TEST_CASE("Game State: Can move piece")
{
    // Create a gameBoard
    GameBoard* gameBoard = new GameBoard{{ Player::white, Player::black }};

    // Create 3 testing pieces and add them to the board
    FiveFivesOneOnePiece* manyPiece = new FiveFivesOneOnePiece{ Player::white }; // 5 5-priority moves to (1, 1) and 1 1-priority move to (1, 1)
    Priority3Piece* white3Piece = new Priority3Piece{ Player::white, std::make_pair(1, 0) };
    Priority3Piece* black3Piece = new Priority3Piece{ Player::black, std::make_pair(2, 0) };
    CHECK(gameBoard->addPieces({ manyPiece, white3Piece, black3Piece }));

    // Create a GameState with 2 players: white and black
    GameState gameState{ gameBoard, { Player::white, Player::black } };
    CHECK(gameState.getCrntPlayer() == Player::white);

    // Make sure you can move manyPiece
    CHECK(gameState.canMovePiece(manyPiece->getPosition(), std::make_pair(1, 1)));
    CHECK(gameState.canMovePiece(0, 0, std::make_pair(1, 1)));
    CHECK(gameState.canMovePiece(manyPiece->getPosition(), 1, 1));
    CHECK(gameState.canMovePiece(0, 0, 1, 1));

    // Make sure you can't move white3Piece
    CHECK(gameState.canMovePiece(white3Piece->getPosition(), std::make_pair(3, 3)) == false);
    CHECK(gameState.canMovePiece(1, 0, std::make_pair(3, 3)) == false);
    CHECK(gameState.canMovePiece(white3Piece->getPosition(), 3, 3) == false);
    CHECK(gameState.canMovePiece(1, 0, 3, 3) == false);

    // Make sure you can't move black3Piece
    CHECK(gameState.canMovePiece(black3Piece->getPosition(), std::make_pair(3, 3)) == false);
    CHECK(gameState.canMovePiece(2, 0, std::make_pair(3, 3)) == false);
    CHECK(gameState.canMovePiece(black3Piece->getPosition(), 3, 3) == false);
    CHECK(gameState.canMovePiece(2, 0, 3, 3) == false);

    // Make sure black3Piece returns its move when checked for moves black has for that piece
    CHECK(gameState.canMovePiece(Player::black, black3Piece->getPosition(), std::make_pair(3, 3)));
    CHECK(gameState.canMovePiece(Player::black, 2, 0, std::make_pair(3, 3)));
    CHECK(gameState.canMovePiece(Player::black, black3Piece->getPosition(), 3, 3));
    CHECK(gameState.canMovePiece(Player::black, 2, 0, 3, 3));

    // Make sure explicitly checking for white's moves doesn't change anything
    CHECK(gameState.canMovePiece(Player::white, black3Piece->getPosition(), std::make_pair(3, 3)) == false);
    CHECK(gameState.canMovePiece(Player::white, 2, 0, std::make_pair(3, 3)) == false);
    CHECK(gameState.canMovePiece(Player::white, black3Piece->getPosition(), 3, 3) == false);
    CHECK(gameState.canMovePiece(Player::white, 2, 0, 3, 3) == false);
}

TEST_CASE("Game State: Move Piece")
{
    // Create a gameBoard
    GameBoard* gameBoard = new GameBoard{{ Player::white, Player::black }};

    // Create 4 testing pieces and add them to the board
    FiveFivesOneOnePiece* manyPiece = new FiveFivesOneOnePiece{ Player::white }; // 5 5-priority moves to (1, 1) and 1 1-priority move to (1, 1)
    FiveDifferentPositionsPiece* diffPiece = new FiveDifferentPositionsPiece{ Player::white, std::make_pair(10, 10) };
    Priority3Piece* white3Piece = new Priority3Piece{ Player::white, std::make_pair(1, 0) };
    Priority3Piece* black3Piece = new Priority3Piece{ Player::black, std::make_pair(2, 0) };
    CHECK(gameBoard->addPieces({ manyPiece, diffPiece, white3Piece, black3Piece }));

    // Create a GameState with 2 players: white and black
    GameState gameState{ gameBoard, { Player::white, Player::black } };
    CHECK(gameState.getCrntPlayer() == Player::white);

    // Check that the initial turn count is 1, since turn is incremented when a
    // gameState is spanwed
    CHECK(gameState.getTurn() == 1);

    /* Check all correct uses of movePiece */

    // Move diffPiece to (11, 11)
    CHECK(gameState.movePiece(diffPiece->getPosition(), std::make_pair(11, 11)));
    CHECK(gameBoard->getPiece(11, 11) == diffPiece);
    CHECK(gameState.getTurn() == 2);

    // Move black3Piece to (3, 3)
    CHECK(gameState.movePiece(2, 0, std::make_pair(3, 3)));
    CHECK(gameBoard->getPiece(3, 3) == black3Piece);
    CHECK(gameState.getTurn() == 3);

    // Move diffPiece to (12, 12)
    CHECK(gameState.movePiece(diffPiece->getPosition(), 12, 12));
    CHECK(gameBoard->getPiece(12, 12) == diffPiece);
    CHECK(gameState.getTurn() == 4);

    // Skip black's turn
    gameState.setNextPlayer();
    CHECK(gameState.getTurn() == 5);

    // Move diffPiece to (13, 13)
    CHECK(gameState.movePiece(12, 12, 13, 13));
    CHECK(gameBoard->getPiece(13, 13) == diffPiece);
    CHECK(gameState.getTurn() == 6);

    // Skip black's turn
    gameState.setNextPlayer();
    CHECK(gameState.getTurn() == 7);

    // Check that indexes works properly by moving manyPiece to (1, 1)
    CHECK(gameState.movePiece(manyPiece->getPosition(), 1, 1, 4));
    CHECK(gameBoard->getPiece(1, 1) == manyPiece);
    CHECK(gameState.getTurn() == 8);

    // Skip black's turn
    gameState.setNextPlayer();
    CHECK(gameState.getTurn() == 9);

    /* Check all edge cases*/

    // Manually move black3Piece back to (2, 0) then try to move it to (3, 3)
    // This should fail because it isn't black's turn
    gameBoard->movePiece(black3Piece->getPosition(), 2, 0);
    CHECK(gameState.movePiece(black3Piece->getPosition(), 3, 3) == false);
    CHECK(gameState.getCrntPlayer() == Player::white);
    CHECK(gameState.getTurn() == 9);

    // Try to move white3Piece to (2, 0)
    // This should fail because white3Piece has too low priority
    CHECK(gameState.movePiece(white3Piece->getPosition(), 2, 0) == false);
    CHECK(gameState.getCrntPlayer() == Player::white);
    CHECK(gameState.getTurn() == 9);

    // Try to move diffPiece from (13, 13) to (13, 13)
    // This should fail because there is a piece at (13, 13), itself!
    // - This logic jump is okay because pieces shouldn't be moving to their own positions
    CHECK(gameState.movePiece(diffPiece->getPosition(), diffPiece->getPosition()) == false);
    CHECK(gameState.getCrntPlayer() == Player::white);
    CHECK(gameState.getTurn() == 9);
}

TEST_CASE("Game State: Can Move")
{
    // Create a gameBoard
    GameBoard* gameBoard = new GameBoard{{ Player::white, Player::black }};

    // Create 4 testing pieces and add them to the board
    Piece* whiteRawPiece = new Piece{ Player::white, std::make_pair(1, 1) };
    Priority5Piece* black5Piece = new Priority5Piece{ Player::black, std::make_pair(5, 5) };
    CHECK(gameBoard->addPieces({ whiteRawPiece, black5Piece }));

    // Create a GameState with 2 players: white and black
    GameState gameState{ gameBoard, { Player::white, Player::black } };
    CHECK(gameState.getCrntPlayer() == Player::white);

    // Make sure white can't move
    CHECK(!gameState.canMove());
    CHECK(!gameState.canMove(Player::white));

    // Make sure black can move
    CHECK(gameState.canMove(Player::black));
}

TEST_CASE("Game State: Get spaces attacked by player")
{
    // Create a gameBoard
    GameBoard* gameBoard = new GameBoard{{ Player::white, Player::black }};

    // Create 3 testing pieces and add them to the board
    OneTwoTwoThreeAttack* whitePiece1 = new OneTwoTwoThreeAttack{ Player::white };
    OneTwoAttack* whitePiece2 = new OneTwoAttack{ Player::white, std::make_pair(0, 1) };
    OneTwoTwoThreeAttack* whitePiece3 = new OneTwoTwoThreeAttack{ Player::white, std::make_pair(0, 2) };
    OneTwoAttack* blackPiece = new OneTwoAttack{ Player::black, std::make_pair(1, 0) };
    CHECK(gameBoard->addPieces({ whitePiece1, whitePiece2, whitePiece3, blackPiece }));

    // Create a GameState with 2 players: white and black
    GameState gameState{ gameBoard, { Player::white, Player::black } };

    // Get all of the spaces attacked by each player
    std::vector<Move::position> whiteAttacks = gameState.getSpacesAttackedByPlayer(Player::white);
    std::vector<Move::position> blackAttacks = gameState.getSpacesAttackedByPlayer(Player::black);
    std::vector<Move::position> defaultAttacks = gameState.getSpacesAttackedByPlayer();

    // Ensure the correct number of spaces were returned (no duplicates and not too few)
    CHECK(whiteAttacks.size() == 4);
    CHECK(blackAttacks.size() == 1);
    CHECK(defaultAttacks.size() == 4);

    // Make sure the vefctors contain the correct spaces
    for(int i = 1; i <= 3; i++) {
        CHECK(std::find(whiteAttacks.begin(), whiteAttacks.end(), std::make_pair(i, i)) != whiteAttacks.end());
        CHECK(std::find(defaultAttacks.begin(), defaultAttacks.end(), std::make_pair(i, i)) != defaultAttacks.end());
    }
    CHECK(std::find(whiteAttacks.begin(), whiteAttacks.end(), std::make_pair(1, 2)) != whiteAttacks.end());
    CHECK(std::find(defaultAttacks.begin(), defaultAttacks.end(), std::make_pair(1, 2)) != defaultAttacks.end());
    CHECK(std::find(blackAttacks.begin(), blackAttacks.end(), std::make_pair(1, 2)) != blackAttacks.end());
}

TEST_CASE("Game State: Get Spaces Attacked By Players")
{
    // Create a gameBoard
    GameBoard* gameBoard = new GameBoard{{ Player::white, Player::black }};

    // Create 3 testing pieces and add them to the board
    OneTwoTwoThreeAttack* whitePiece1 = new OneTwoTwoThreeAttack{ Player::white };
    OneTwoAttack* whitePiece2 = new OneTwoAttack{ Player::white, std::make_pair(0, 1) };
    OneTwoTwoThreeAttack* blackPiece1 = new OneTwoTwoThreeAttack{ Player::black, std::make_pair(0, 2) };
    OneTwoAttack* blackPiece2 = new OneTwoAttack{ Player::black, std::make_pair(1, 0) };
    OneThreeAttack* silverPiece = new OneThreeAttack{ Player::silver, std::make_pair(10, 10) };
    CHECK(gameBoard->addPieces({ whitePiece1, whitePiece2, blackPiece1, blackPiece2, silverPiece }));

    // Create a GameState with 2 players: white and black
    GameState gameState{ gameBoard, { Player::white, Player::black, Player::silver} };

    // Get all spaces attacked by white+black and by black+silver
    std::vector<Move::position> whiteBlackAttacks = gameState.getSpacesAttackedByPlayers({ Player::white, Player::black });
    std::vector<Move::position> blackSilverAttacks = gameState.getSpacesAttackedByPlayers({ Player::black, Player::silver });
    std::vector<Move::position> attackingSilver = gameState.getAttackedSpaces(Player::silver);
    std::vector<Move::position> attackingDefault = gameState.getAttackedSpaces();

    // Ensure the correct number of spaces were returned (no duplicates and not too few)
    CHECK(whiteBlackAttacks.size() == 4);
    CHECK(blackSilverAttacks.size() == 5);
    CHECK(attackingSilver.size() == 4);
    CHECK(attackingDefault.size() == 5);

    // Make sure the vefctors contain the correct spaces
    for(int i = 1; i <= 3; i++) {
        CHECK(std::find(whiteBlackAttacks.begin(), whiteBlackAttacks.end(), std::make_pair(i, i)) != whiteBlackAttacks.end());
        CHECK(std::find(blackSilverAttacks.begin(), blackSilverAttacks.end(), std::make_pair(i, i)) != blackSilverAttacks.end());
        CHECK(std::find(attackingSilver.begin(), attackingSilver.end(), std::make_pair(i, i)) != attackingSilver.end());
        CHECK(std::find(attackingDefault.begin(), attackingDefault.end(), std::make_pair(i, i)) != attackingDefault.end());
    }
    CHECK(std::find(whiteBlackAttacks.begin(), whiteBlackAttacks.end(), std::make_pair(1, 2)) != whiteBlackAttacks.end());
    CHECK(std::find(blackSilverAttacks.begin(), blackSilverAttacks.end(), std::make_pair(1, 2)) != blackSilverAttacks.end());
    CHECK(std::find(attackingSilver.begin(), attackingSilver.end(), std::make_pair(1, 2)) != attackingSilver.end());
    CHECK(std::find(attackingDefault.begin(), attackingDefault.end(), std::make_pair(1, 2)) != attackingDefault.end());
    CHECK(std::find(whiteBlackAttacks.begin(), whiteBlackAttacks.end(), std::make_pair(1, 3)) == whiteBlackAttacks.end());
    CHECK(std::find(blackSilverAttacks.begin(), blackSilverAttacks.end(), std::make_pair(1, 3)) != blackSilverAttacks.end());
    CHECK(std::find(attackingSilver.begin(), attackingSilver.end(), std::make_pair(1, 3)) == attackingSilver.end());
    CHECK(std::find(attackingDefault.begin(), attackingDefault.end(), std::make_pair(1, 3)) != attackingDefault.end());
    
    // Check isAttacked()
    CHECK(gameState.isAttacked(Player::silver, 1, 2));
    CHECK(gameState.isAttacked(Player::silver, std::make_pair(1, 3)) == false);
    CHECK(gameState.isAttacked(1, 2));
    CHECK(gameState.isAttacked(std::make_pair(1, 3)));
    CHECK(gameState.isAttacked(5, 5) == false);
}

TEST_CASE("Game State: Min Priority")
{
    // Create a gameBoard
    GameBoard* gameBoard = new GameBoard{{ Player::white, Player::black }};

    // Create a regular white piece, a regular black piece, and a priority blocking white piece
    // and add them to the board
    FiveFivesOneOnePiece* whiteNormalPiece = new FiveFivesOneOnePiece{ Player::white }; // 5 5-priority moves to (1, 1) and 1 1-priority move to (1, 1)
    Priority3Piece* blackNormalPiece = new Priority3Piece{ Player::black, std::make_pair(2, 0) };
    BigPriority* whitePriorityBlock = new BigPriority { Player::white, std::make_pair(4, 4) };
    CHECK(gameBoard->addPieces({ whiteNormalPiece, blackNormalPiece, whitePriorityBlock}));

    // Create a GameState with 2 players: white and black
    GameState gameState{ gameBoard, { Player::white, Player::black } };
    CHECK(gameState.getCrntPlayer() == Player::white);

    // Make sure white cannot move because of the priority blocker
    CHECK(gameState.getPriorityOfPlayer(Player::white) == 0);
    CHECK(gameState.getMovesOfPiece(whiteNormalPiece->getPosition(), std::make_pair(1, 1)).size() == 0);
    CHECK(gameState.getMovesOfPiece(0, 0, std::make_pair(1, 1)).size() == 0);
    CHECK(gameState.getMovesOfPiece(whiteNormalPiece->getPosition(), 1, 1).size() == 0);
    CHECK(gameState.getMovesOfPiece(0, 0, 1, 1).size() == 0);
    
    // Make sure black can still move
    CHECK(gameState.getPriorityOfPlayer(Player::black) == 3);
    CHECK(gameState.getMovesOfPiece(Player::black, blackNormalPiece->getPosition(), std::make_pair(3, 3)).size() == 1);
    CHECK(gameState.getMovesOfPiece(Player::black, 2, 0, std::make_pair(3, 3)).size() == 1);
    CHECK(gameState.getMovesOfPiece(Player::black, blackNormalPiece->getPosition(), 3, 3).size() == 1);
    CHECK(gameState.getMovesOfPiece(Player::black, 2, 0, 3, 3).size() == 1);

    // Add a priority blocking black piece
    BigPriority* blackPriorityBlock = new BigPriority { Player::black, std::make_pair(6, 6) };
    CHECK(gameBoard->addPiece(blackPriorityBlock));

    // Make sure black now has no moves
    CHECK(gameState.getPriorityOfPlayer(Player::black) == 0);
    CHECK(gameState.getMovesOfPiece(Player::black, blackNormalPiece->getPosition(), std::make_pair(3, 3)).size() == 0);
    CHECK(gameState.getMovesOfPiece(Player::black, 2, 0, std::make_pair(3, 3)).size() == 0);
    CHECK(gameState.getMovesOfPiece(Player::black, blackNormalPiece->getPosition(), 3, 3).size() == 0);
    CHECK(gameState.getMovesOfPiece(Player::black, 2, 0, 3, 3).size() == 0);
}