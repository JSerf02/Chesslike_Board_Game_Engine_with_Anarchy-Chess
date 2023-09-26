#ifndef TRY_CAPTURE_PIECE_ACTION_H
#define TRY_CAPTURE_PIECE_ACTION_H

#include <stdlib.h>
#include <utility>
#include <vector>
#include <iostream>
#include <functional>

#include "GameBoard.h"
#include "Move.h"
#include "Piece.h"
#include "CapturePieceAction.h"

namespace logic {
    /* An operation that can be completed immediately before or after a move */
    class TryCapturePieceAction : public CapturePieceAction
    {
        private:
            /* 
            * Stores whether or not a capture occured during the last call of this
            * action
            */
            bool justRemoved = false;

        public:
            /* Inherit the constructor */
            using CapturePieceAction::CapturePieceAction;

            /*
            * Removes the piece at removedPos + end if there is a piece there
            */
            bool callAction(Move::position end, GameBoard* board) override;

            /*
            * Unremoves the previously removed piece if one was removed
            */
            bool reverseAction(GameBoard* board) override;

            /*
            * Captures the removed piece
            */
            void applySymptomaticEffects(GameBoard* board) override;
    };
}
#endif