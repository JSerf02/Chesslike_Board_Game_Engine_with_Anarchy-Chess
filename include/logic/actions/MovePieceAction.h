#ifndef MOVE_PIECE_ACTION_H
#define MOVE_PIECE_ACTION_H

#include <stdlib.h>
#include <utility>
#include <vector>
#include <iostream>
#include <functional>

#include "GameBoard.h"
#include "Move.h"
#include "Piece.h"

namespace logic {
    /* An action that moves a piece from one position to another */
    class MovePieceAction : public Action
    {            
        private:
            /*
            * The original location of the piece relative to the end position of 
            * the move
            */
            Move::position startPos{};

            /*
            * The ending position of the piece after this action relative to the 
            * end position of the move
            */
            Move::position endPos{};

            /*
            * The end position of the move used during the most recent call of 
            * this action
            */
            Move::position appliedEnd{};

            /*
            * The piece that was moved during the most recent call of this action
            */
            Piece* movedPiece{};

        public:
            /*
            * Create an action of this type with a starting position of startPos
            * and an ending position of endPos
            * - startPos and endPos are relative to the ending location of the move
            *   that calls this action
            */
            MovePieceAction(Move::position endPos = std::make_pair(0, 0)) : endPos{endPos}{}
            MovePieceAction(Move::position startPos, Move::position endPos) :
                startPos{startPos}, endPos{endPos}{}

            /*
            * Moves a piece from end + startPos to end + endPos, sets appliedEnd = end,
            * and stores the piece that was moved
            *
            * Returns:
            * - true if the piece was successfully moved 
            * - false if the piece could not move
            */
            bool callAction(Move::position end, GameBoard* board) override;

            /*
            * Moves a piece from appliedEnd + endPos to appliedEnd + startPos
            *
            * Returns:
            * - true if the piece was successfully returned to its original position
            * - false if something went wrong, which can happen if the board was modified
            *   between calls to callAction() and reverseAction()
            */
            bool reverseAction(GameBoard* board) override;

            /*
            * Validates the move
            */
            void applySymptomaticEffects(GameBoard* board) override;
    };
}
#endif