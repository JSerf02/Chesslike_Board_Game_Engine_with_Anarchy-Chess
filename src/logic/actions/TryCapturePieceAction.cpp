#include "Action.h"
#include "GameBoard.h"
#include "Move.h"
#include "TryCapturePieceAction.h"
#include "CapturePieceAction.h"

namespace logic {
    // See TryCapturePieceAction.h
    bool TryCapturePieceAction::callAction(Move::position end, GameBoard* board)
    {
        if(!board) {
            return false;
        }
        Move::position removePosition = std::make_pair(end.first + removePos.first, end.second + removePos.second);
        if(!board->occupiedOnBoard(removePosition)) {
            justRemoved = false;
            return true;
        }
        justRemoved = CapturePieceAction::callAction(end, board);
        return justRemoved;
    }

    // See TryCapturePieceAction.h
    bool TryCapturePieceAction::reverseAction(GameBoard* board)
    {
        if(!justRemoved) {
            return true;
        }
        justRemoved = false;
        return CapturePieceAction::reverseAction(board);
    }

    // See TryCapturePieceAction.h
    void TryCapturePieceAction::applySymptomaticEffects(GameBoard* board)
    {
        if(!justRemoved) {
            return;
        }
        justRemoved = false;
        CapturePieceAction::applySymptomaticEffects(board);
    }
}