#include "Action.h"
#include "GameBoard.h"
#include "Move.h"
#include "CapturePieceAction.h"

namespace logic {
    // See CapturePieceAction.h
    bool CapturePieceAction::callAction(Move::position end, GameBoard* board)
    {
        if(!board) {
            return false;
        }
        Move::position removePosition = std::make_pair(end.first + removePos.first, end.second + removePos.second);
        Piece* newPiece = board->getPiece(removePosition);
        if(!newPiece || newPiece->getPlayerAccess(player)) {
            return false;
        }
        removedPiece = newPiece;
        return board->removePiece(removePosition);
    }

    // See CapturePieceAction.h
    void CapturePieceAction::applySymptomaticEffects(GameBoard* board)
    {
        if(!board) {
            return;
        }
        board->captureRemovedPiece(removedPiece);
    }
}