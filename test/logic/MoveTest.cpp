#include <vector>
#include <iostream>

#include "doctest.h"
#include "Move.h"
#include "GameBoard.h"
#include "GameState.h"

TEST_CASE("Move: Get and set priority") 
{
    // Create a Move object
    Move testMove{};
    
    // Check if the default priority is properly initialized to 0
    CHECK(testMove.getPriority() == 0);

    // Check if setPriority() properly sets the priority
    // Only one case is necessary as functionality is identical for all ints
    testMove.setPriority(10);
    CHECK(testMove.getPriority() == 10);
}

/*
 * A callback function to test the move's onMove functionality
 *
 * Using a global variable to check whether the function was called without
 *   modifying the board because the board module is not written yet
*/
static bool testCallbackCalled = false;
void testCallback(GameState& gameState) 
{
    testCallbackCalled = true;
}

TEST_CASE("Move: Set and call onMove() callback") 
{
    // Create a Move object
    Move testMove{};

    // Create an empty GameState to use as a parameter for the onMove functions
    GameBoard board{};
    GameState testState{&board};

    // Make sure nothing is called, nothing crashes, and false is returned
    // when the default (null) onMove() is called
    CHECK(testMove.callOnMove(testState) == false);
    CHECK(testCallbackCalled == false);

    // Set testCallback as the new onMove() and make sure it is called and that 
    // callOnMove() returns true
    testMove.setOnMove(testCallback);
    CHECK(testMove.callOnMove(testState) == true);
    CHECK(testCallbackCalled == true);
}

using position = Move::position;
TEST_CASE("Move: Positions") 
{
    // Create a Move object
    Move testMove{};

    // Check if positions are initialized properly and if getPositions works
    const std::vector<position>& positions = testMove.getPositions();
    REQUIRE(positions.size() == 0);

    SUBCASE("Move: Add and get positions") 
    {
        // Add a new position
        testMove.addPosition(std::make_pair(0, 0));

        // Get the positions and check if the new position is correctly added
        CHECK(positions.size() == 1);
        CHECK(positions[0] == std::make_pair(0, 0));

        // Add two more positions
        testMove.addPositions({std::make_pair(1, 1), std::make_pair(2, 2)});

        // Make sure the positions were added properly into the right locations
        CHECK(positions.size() == 3);
        CHECK(positions[0] == std::make_pair(0, 0));
        CHECK(positions[1] == std::make_pair(1, 1));
        CHECK(positions[2] == std::make_pair(2, 2));
    }

    SUBCASE("Move: Find and Contains position") 
    {
        // Make sure false is returned when the position isn't in the move
        CHECK(testMove.containsPosition(std::make_pair(0, 0)) == false);

        // Add a position and make sure true is returned when the position is there
        testMove.addPosition(std::make_pair(0, 0));
        CHECK(testMove.findPositionIdx(std::make_pair(0, 0)) == 0);
        CHECK(testMove.containsPosition(std::make_pair(0, 0)));
        
        // Ensure proper results when the position is not in the list
        CHECK(testMove.findPositionIdx(std::make_pair(1, 1)) == -1);
        CHECK(testMove.containsPosition(std::make_pair(1, 1)) == false); 

        // Confirm it works when there are many positions
        for(int i = 1; i < 10; i++) {
            testMove.addPosition(std::make_pair(i, i));
        }
        CHECK(positions.size() == 10);
        CHECK(testMove.findPositionIdx(std::make_pair(3, 3)) == 3);
        CHECK(testMove.containsPosition(std::make_pair(5, 5))); 
        CHECK(testMove.findPositionIdx(std::make_pair(20, 20)) == -1);
    }

    SUBCASE("Move: Remove positions")
    {
        // Add 10 positions and ensure the adding worked properly
        for(int i = 0; i < 10; i++) {
            testMove.addPosition(std::make_pair(i, i));
        }
        CHECK(positions.size() == 10);

        // Test removal at index when the index correct
        CHECK(positions[0] == std::make_pair(0, 0));
        CHECK(testMove.removePosition(0)); // Make sure the function returns true
        CHECK(positions[0] == std::make_pair(1, 1));

        // Test removal at index when the index is incorrect
        CHECK(testMove.removePosition(-5) == false);
        CHECK(testMove.removePosition(200) == false);

        // Test removal of specific position if position is present
        CHECK(testMove.removePosition(std::make_pair(5, 5)));
        CHECK(testMove.containsPosition(std::make_pair(5, 5)) == false);

        // Test removal of specific position if position is not present
        CHECK(testMove.removePosition(std::make_pair(20, 20)) == false);
    }

    SUBCASE("Move: Clear positions") 
    {
        // Add 10 positions and ensure the adding worked properly
        for(int i = 0; i < 10; i++) {
            testMove.addPosition(std::make_pair(i, i));
        }
        CHECK(positions.size() == 10);

        // Clear the positions and make sure it actually cleared
        testMove.clearPositions();
        CHECK(positions.size() == 0);
    }
}

