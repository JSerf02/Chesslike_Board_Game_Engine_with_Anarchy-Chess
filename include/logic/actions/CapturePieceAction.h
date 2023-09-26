#ifndef CAPTURE_PIECE_ACTION_H
#define CAPTURE_PIECE_ACTION_H

#include <stdlib.h>
#include <utility>
#include <vector>
#include <iostream>
#include <functional>

#include "GameBoard.h"
#include "Move.h"
#include "Piece.h"
#include "RemovePieceAction.h"

namespace logic {
    using Player = Piece::Player;

    /* An operation that can be completed immediately before or after a move */
    class CapturePieceAction : public RemovePieceAction
    {
        protected:
            /*
            * The player that is removing the piece
            */
            Player player{};
            
        public:
            /* 
            * Creates an action of this type with the player assigned 
            */
            CapturePieceAction(Player player, Move::position removePos = std::make_pair(0, 0)) :
                player{player}, RemovePieceAction(removePos){}


            /*
            * Removes the piece at end + removePos
            *
            * Returns:
            * - true if the piece was successfully removed
            * - false if there is no piece to remove or if removal failed
            */
            bool callAction(Move::position end, GameBoard* board) override;

            /*
            * Captures the most recently removed piece
            */
            void applySymptomaticEffects(GameBoard* board) override;
    };
}
#endif