#ifndef ACTION_H
#define ACTION_H

#include <stdlib.h>
#include <utility>
#include <vector>
#include <iostream>
#include <functional>

#include "GameBoard.h"
#include "Move.h"

#define make_action(constructor) (std::shared_ptr<Action>(static_cast<Action*>(constructor)))

namespace logic {
    /* 
    * An abstract class representing an operation that can be completed 
    * immediately before or after a move 
    * */
    class Action 
    {
        public:
            /*
            * Call the action
            * - Default implementation does nothing
            * - Any interaction done with this function must be reversable
            * 
            * Returns:
            * - true if the action was successfully called
            * - false if the action was not successfully called
            *   - In such a case, the action should be asymptomatic and the board 
            *     should not change
            */
            virtual bool callAction(Move::position end, GameBoard* board){ return true; };

            /*
            * Reverses the effects of callAction
            * - Default implementation does nothing
            * 
            * Returns:
            * - true if the effects were successfully undone
            * - false if the action could not be reversed
            */
            virtual bool reverseAction(GameBoard* board){ return true; };

            /*
            * Applies any effects of the action that cannot be reversed
            * - Default implementation does nothing
            * - Implementations should be able to withstand changes in the board state
            *   between callAction() calls and calls of this function
            */
            virtual void applySymptomaticEffects(GameBoard* board){};
    };
}
#endif