#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>

#include "Move.h"
#include "GameState.h"

namespace logic {
    using position = Move::position;

    // See Move.h
    std::ostream& operator<<(std::ostream& os, const Move& move)
    {
        os << "{";
        for(position curPos : move.positions) {
            os << "(" << curPos.first << ", " << curPos.second << ")";
        }
        os << "}, ";
        os << "Priority: " << move.priority << ", ";
        os << "onMove() is " << ((move.onMove != nullptr) ? "" : "not ") << "set\n";
        return os;
    }

    // See Move.h
    int Move::getPriority() 
    {
        return priority;
    }

    // See Move.h
    void Move::setPriority(int newPriority) 
    {
        priority = newPriority;
    }

    // See Move.h
    void Move::setOnMove(void (*onMoveCallback)(GameState&)) 
    {
        onMove = onMoveCallback;
    }

    // See Move.h
    bool Move::callOnMove(GameState& gameState)
    {
        // Return if callback is null
        if(!onMove) {
            return false;
        }

        // Call onMove
        (*onMove)(gameState);
        return true;
    }

    // See Move.h
    const std::vector<position>& Move::getPositions() 
    {
        return positions;
    }

    // See Move.h
    void Move::addPosition(position newPosition) 
    {
        positions.push_back(newPosition);
    }

    // See Move.h
    void Move::addPositions(std::vector<position> newPositions)
    {
        for(position newPosition : newPositions) {
            addPosition(newPosition);
        }
    }

    // See Move.h
    int Move::findPositionIdx(position checkPos) 
    {
        // Loop through all of the elements and check for equality
        for(int i = 0; i < positions.size(); i++) {
            if((positions[i].first == checkPos.first) && (positions[i].second == checkPos.second)) {
                return i;
            }
        }
        // Return -1 if the position is not found
        return -1;
    }

    // See Move.h
    bool Move::containsPosition(position checkPos) 
    {
        return findPositionIdx(checkPos) >= 0;
    }

    // See Move.h
    bool Move::removePosition(int idx) 
    {
        // Check for invalid inputs
        if(idx < 0 || idx >= positions.size()) {
            return false;
        }

        // Remove the position
        positions.erase(positions.begin() + idx);
        return true;
    }

    // See Move.h
    bool Move::removePosition(position removePos) 
    {
        return removePosition(findPositionIdx(removePos));
    }

    // See Move.h
    void Move::clearPositions() 
    {
        positions.clear();
    }
}
