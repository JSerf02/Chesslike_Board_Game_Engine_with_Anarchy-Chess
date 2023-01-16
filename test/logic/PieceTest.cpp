#include <vector>
#include <iostream>
#include <utility>

#include "doctest.h"
#include "Piece.h"
#include "Move.h"
#include "GameBoard.h"
#include "GameState.h"
#include "TestPieces.h"

using namespace testing;
using namespace logic;

using Player = Piece::Player;

TEST_CASE("Piece: Get player access with default values")
{
    // Create a new piece
    Piece testPiece{};
    
    // Ensure default access of each player type is false
    CHECK(testPiece.getPlayerAccess(Player::white) == false);
    CHECK(testPiece.getPlayerAccess(Player::black) == false);
    CHECK(testPiece.getPlayerAccess(Player::silver) == false);
    CHECK(testPiece.getPlayerAccess(Player::gold) == false);
}

TEST_CASE("Piece: Get player access with added pieces") 
{
    // Create a new piece
    Piece testPiece{};
    
    // Add an arbitrary player to the piece
    testPiece.addPlayer(Player::black);

    // Check that access was only given to the new player
    CHECK(testPiece.getPlayerAccess(Player::black));
    CHECK(testPiece.getPlayerAccess(Player::white) == false);
    CHECK(testPiece.getPlayerAccess(Player::silver) == false);
    CHECK(testPiece.getPlayerAccess(Player::gold) == false);
}

TEST_CASE("Piece: Add multiple players")
{
    // Create a new piece
    Piece testPiece{};

    // Add 2 players to the piece
    testPiece.addPlayers({ Player::white, Player::black });

    // Check that access was given to both players and no one else
    CHECK(testPiece.getPlayerAccess(Player::white));
    CHECK(testPiece.getPlayerAccess(Player::black));
    CHECK(testPiece.getPlayerAccess(Player::silver) == false);
    CHECK(testPiece.getPlayerAccess(Player::gold) == false); 
}

TEST_CASE("Piece: Add players through constructor")
{
    // Create a new piece with 2 players
    Piece testPiece{ { Player::white, Player::black } };

    // Check that access was given to both players and no one else
    CHECK(testPiece.getPlayerAccess(Player::white));
    CHECK(testPiece.getPlayerAccess(Player::black));
    CHECK(testPiece.getPlayerAccess(Player::silver) == false);
    CHECK(testPiece.getPlayerAccess(Player::gold) == false); 

    // Create a new piece with 1 player
    Piece testPiece2{ Player::white };

    // Check that access was given only to the intended players
    CHECK(testPiece2.getPlayerAccess(Player::white));
    CHECK(testPiece2.getPlayerAccess(Player::black) == false);
    CHECK(testPiece2.getPlayerAccess(Player::silver) == false);
    CHECK(testPiece2.getPlayerAccess(Player::gold) == false); 
}

TEST_CASE("Piece: Get all players access") 
{
    // Create a new piece
    Piece testPiece{};
    
    // Add an arbitrary player to the piece
    testPiece.addPlayer(Player::black);

    // Get all of the players' accesses
    auto allAccess = testPiece.getAllPlayersAccess();

    // Check that access was only given to the new player
    CHECK(allAccess[Player::black]);
    CHECK(allAccess[Player::white] == false);
    CHECK(allAccess[Player::silver] == false);
    CHECK(allAccess[Player::gold] == false);
}

TEST_CASE("Piece: Remove player access") 
{
    // Create a new piece
    Piece testPiece{};
    
    // Add an arbitrary player to the piece and make sure its access updated properly
    testPiece.addPlayer(Player::gold); 
    CHECK(testPiece.getPlayerAccess(Player::gold));
    
    // Remove the player from the piece and make sure its access updated properly
    testPiece.removePlayer(Player::gold);
    CHECK(testPiece.getPlayerAccess(Player::gold) == false);
}

TEST_CASE("Piece: Get position") 
{
    // Initialize a piece to an arbitrary position
    Move::position defaultPosition = std::make_pair(1, 5);
    Piece testPiece{defaultPosition};

    // Ensure getPosition() returns the correct position
    CHECK(testPiece.getPosition() == defaultPosition);
}

TEST_CASE("Piece: Change Position") 
{
    // Create a Piece for testing
    Piece testPiece{};

    // Move using the int variation on the function and ensure the piece properly moved
    testPiece.changePosition(2, 2);
    CHECK(testPiece.getPosition() == std::make_pair(2, 2));

    // Move using the position variation on the function and ensure the piece properly moved
    Move::position newPosition = std::make_pair(3, 4);
    testPiece.changePosition(newPosition);
    CHECK(testPiece.getPosition() == newPosition);
}

// A class for testing max piece priority
class Max5Piece : public Piece
{
    public:
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {{0}, {1}, {5}};
        } 
};

// A class for testing piece priority when there are no moves
class NoMovesPiece : public Piece 
{
    public:
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {};
        } 
};

TEST_CASE("Piece: Get maximum priority of all moves")
{
    // Create a gameState and a testPiece
    GameState gameState{};
    Max5Piece max5Piece{};
    NoMovesPiece noMovesPiece{};

    // Make sure the max priority of max5Piece's moves is 5
    CHECK(max5Piece.getMaxPriorityOfMoves(gameState) == 5);
    
    // Make sure the max priority of noMovesPIece is 0
    CHECK(noMovesPiece.getMaxPriorityOfMoves(gameState) == 0);
}

TEST_CASE("Piece: Get Attacked Spaces")
{
    // Create a GameState for testing
    GameState gameState{};

    // Create two pieces for attacking
    OneTwoTwoThreeAttack attackPiece{}; // {{(1,1)}, {(2, 2)}, {(2, 2), (3, 3)}}
    Piece regularPiece{}; // {}
    
    CHECK(attackPiece.getAttackedSpaces(gameState).size() == 3);
    CHECK(regularPiece.getAttackedSpaces(gameState).size() == 0);
}

TEST_CASE("Piece: Value initialization and get value")
{
    // Check the default value is properly set to 0
    Piece testPiece{};
    CHECK(testPiece.getValue() == 0);

    // Check the value is properly initialized
    Piece testPiece2{std::make_pair(0, 0), 5};
    CHECK(testPiece2.getValue() == 5);
}

TEST_CASE("Piece: Set value and get non-default value")
{
    // Create a Piece for testing
    Piece testPiece{};

    // Change the piece's value and ensure it properly adjusted
    testPiece.setValue(7);
    CHECK(testPiece.getValue() == 7);
}

TEST_CASE("Piece: Manage on-board")
{
    // Create a Piece for testing
    Piece piece{};

    // Make sure the piece is not on a board by default
    CHECK(piece.getOnBoard() == false);

    // Set onBoard to true and make sure it updates properly
    piece.setOnBoard(true);
    CHECK(piece.getOnBoard());

    // Set onBoard to false and make sure it updates properly
    piece.setOnBoard(false);
    CHECK(piece.getOnBoard() == false);
}