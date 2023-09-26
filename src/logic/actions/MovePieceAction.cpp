#include "Action.h"
#include "GameBoard.h"
#include "Move.h"
#include "MovePieceAction.h"

namespace logic {
    // See MovePieceAction.h
    bool MovePieceAction::callAction(Move::position end, GameBoard* board)
    {
        if(!board) {
            return false;
        }
        appliedEnd = end;
        movedPiece = board->getPiece(std::make_pair(end.first + startPos.first, end.second + startPos.second));
        return board->movePiece(
            std::make_pair(end.first + startPos.first, end.second + startPos.second),
            std::make_pair(end.first + endPos.first, end.second + endPos.second)
        );
    }

    // See MovePieceAction.h
    bool MovePieceAction::reverseAction(GameBoard* board)
    {
        if(!board) {
            return false;
        }
        return board->movePiece(
            std::make_pair(appliedEnd.first + endPos.first, appliedEnd.second + endPos.second),
            std::make_pair(appliedEnd.first + startPos.first, appliedEnd.second + startPos.second)
        );
    }

    // See MovePieceAction.h
    void MovePieceAction::applySymptomaticEffects(GameBoard* board)
    {
        if(!board || !movedPiece) {
            return;
        }
        movedPiece->validateMove();
    }
}