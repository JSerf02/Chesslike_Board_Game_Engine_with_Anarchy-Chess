#ifndef MOVE_H
#define MOVE_H

#include <stdlib.h>
#include <utility>
#include <vector>
#include <iostream>

class GameBoard;
class Move {
    public:
        /*
         * A position on the game board
        */
        using position = std::pair<int, int>;
    private:
        /*
         * The positions this move allows
        */ 
        std::vector<position> positions{};

        /*
         * The move's priority (higher means better)
         * - Priority 0: Regular moves
         * - Priority 10: En Pessant
        */
        int priority{};

        /*
         * A callback function that is called whenever a player uses one of these moves
         * -- If null, will be ignored
        */
        void (*onMove)(GameBoard&){nullptr};

    public:
        /*
         * Constructor: Initialize priority and onMove() callback function
        */
        Move(int movePriority = 0, void (*onMoveCallback)(GameBoard&) = nullptr) : 
            priority{movePriority}, onMove{onMoveCallback}{ }
        
        /*
         * Allow printing of moves for debugging
        */
        friend std::ostream& operator<<(std::ostream& os, const Move& move);
        
        /*
         * Returns the current priority 
        */
        int getPriority();

        /* 
         * Changes the priority to a new value
        */
        void setPriority(int newPriority);

        /*
         * Changes the onMove() callback to a new callback function
        */
        void setOnMove(void (*onMoveCallback)(GameBoard&));

        /*
         * Calls the onMove() callback function if one is defined
         * - Does nothing if the onMove() callback is null
         * 
         * Parameters:
         * - board: The GameBoard object of the current game
         * 
         * Returns:
         * - True if the function was called
         * - False if onMove() was null and nothing was called
        */
        bool callOnMove(GameBoard& board);

        /*
         * Returns a reference to the positions allowed by this move
        */
        const std::vector<position>& getPositions();

        /* 
         * Adds a position to the move
        */
        void addPosition(position newPos);

        /*
         * Searches the move's positions for the provided positions using a linear
         * search (O(n) complexity) because positions aren't ordered
         *
         * Parameters:
         * - checkPos: The position to search for
         * 
         * Returns:
         * - The index of the position if it is in the move
         * - -1 if the position is not in the move
        */
        int findPositionIdx(position checkPos);
        
        /*
         * Searches the move's positions for the provided positions using a linear
         * search (O(n) complexity) because positions aren't ordered
         *
         * Parameters:
         * - checkPos: The position to search for
         * 
         * Returns:
         * - true if the move contains the provided position
         * - false if the move does not contain the provided position
        */
        bool containsPosition(position checkPos);

        /*
         * Removes the position at the provided index from the move
         * Returns:
         * - true if something was removed 
         * - false if something went wrong and nothing was removed
        */
        bool removePosition(int idx);

        /*
         * Searches for a position and removes it if it is in the move
         * 
         * Parameters:
         * - removePos: The position to search for and remove
         * 
         * Returns:
         * - true if something was removed
         * - false if something went wrong and nothing was removed
        */
        bool removePosition(position removePos);

        /*
         * Removes all positions from the move
        */
        void clearPositions();
};

#endif