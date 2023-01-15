#include <vector>
#include <iostream>

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
    GameState gameState{{}, players};

    // Make sure the first player is black, the second gold, and the third silver
    CHECK(gameState.getPlayer(0) == Player::black);
    CHECK(gameState.getCrntPlayer() == Player::black);
    CHECK(gameState.getPlayer(1) == Player::gold);
    CHECK(gameState.getPlayer(2) == Player::silver);
}

TEST_CASE("Game State: Set crnt and next players")
{
    // Create a GameState with 3 players: black, gold, and silver
    std::vector<Player> players = { Player::black, Player::gold, Player::silver };
    GameState gameState{{}, players};

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