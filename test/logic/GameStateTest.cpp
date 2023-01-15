#include <vector>
#include <iostream>
#include <algorithm>

#include "doctest.h"
#include "Move.h"
#include "Piece.h"
#include "GameBoard.h"
#include "TestBoard.h"
#include "GameState.h"

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

TEST_CASE("Game State: Get pieces controlled by player")
{
    // Create a gameBoard
    GameBoard* gameBoard = new GameBoard{};

    // Add 3 white pieces and 2 black piecces to the board
    Piece* whitePiece1 = new Piece{Player::white};
    Piece* whitePiece2 = new Piece{Player::white, std::make_pair(1, 0)};
    Piece* whitePiece3 = new Piece{Player::white, std::make_pair(2, 0)};
    Piece* blackPiece1 = new Piece{Player::black, std::make_pair(0, 1)};
    Piece* blackPiece2 = new Piece{Player::black, std::make_pair(0, 2)};
    CHECK(gameBoard->addPieces({ whitePiece1, whitePiece2, whitePiece3, blackPiece1, blackPiece2 }));

    // Create a GameState with 2 players: white and black
    GameState gameState{ gameBoard, { Player::white, Player::black } };

    // Get the white and black pieces and also get the pieces of the current player (should be white)
    std::vector<Move::position> whitePieces = gameState.getPiecesOfPlayer(Player::white);
    std::vector<Move::position> crntPlayerPieces = gameState.getPiecesOfCrntPlayer();
    std::vector<Move::position> blackPieces = gameState.getPiecesOfPlayer(Player::black);

    // Make sure pieces were properly returned
    for(int i = 0; i <=2; i++) { // (0, 0), (1, 0), and (2, 0) are in whitePieces and crntPlayerPieces
        CHECK(std::find(whitePieces.begin(), whitePieces.end(), std::make_pair(i, 0)) != whitePieces.end());
        CHECK(std::find(crntPlayerPieces.begin(), crntPlayerPieces.end(), std::make_pair(i, 0)) != crntPlayerPieces.end());
        CHECK(std::find(blackPieces.begin(), blackPieces.end(), std::make_pair(i, 0)) == blackPieces.end());
    }
    for(int j = 1; j <= 2; j++) { // (0, 1), (0, 2) are in blackPieces
        CHECK(std::find(whitePieces.begin(), whitePieces.end(), std::make_pair(0, j)) == whitePieces.end());
        CHECK(std::find(crntPlayerPieces.begin(), crntPlayerPieces.end(), std::make_pair(0, j)) == crntPlayerPieces.end());
        CHECK(std::find(blackPieces.begin(), blackPieces.end(), std::make_pair(0, j)) != blackPieces.end());
    }
}
