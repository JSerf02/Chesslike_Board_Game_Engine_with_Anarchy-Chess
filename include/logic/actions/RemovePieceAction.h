#ifndef REMOVE_PIECE_ACTION_H
#define REMOVE_PIECE_ACTION_H

#include <stdlib.h>
#include <utility>
#include <vector>
#include <iostream>
#include <functional>

#include "GameBoard.h"
#include "Move.h"
#include "Piece.h"

namespace logic {
    /* An action that removes a piece from the board */
    class RemovePieceAction : public Action
    {            
        protected:
            /*
            * The position relative to the end position of the move of the piece
            * to remove
            */
            Move::position removePos{};

            /*
            * The piece removed by the most recent call of this action
            */
            Piece* removedPiece{};

        public:
            /*
            * Create an action of this type with the removal position
            * - The removal position is the position relative to the end position of
            *   the move of the piece that should be removed
            */
            RemovePieceAction(Move::position removePos = std::make_pair(0, 0)) :
                removePos{removePos}{}

            /*
            * Removes the piece at end + removePos
            *
            * Returns:
            * - true if the piece was successfully removed
            * - false if there is no piece to remove or if removal failed
            */
            bool callAction(Move::position end, GameBoard* board) override;

            /*
            * Unremoves the previously removed piece
            *
            * Returns:
            * - true if the removal was successfully reversed
            * - false if something went wrong and the piece is still removed
            */
            bool reverseAction(GameBoard* board) override;
    };
}
#endif