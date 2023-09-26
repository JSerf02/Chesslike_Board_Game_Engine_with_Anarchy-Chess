#include "Action.h"
#include "GameBoard.h"
#include "Move.h"
#include "RemovePieceAction.h"

namespace logic {
    // See RemovePieceAction.h
    bool RemovePieceAction::callAction(Move::position end, GameBoard* board)
    {
        if(!board) {
            return false;
        }
        Move::position removePosition = std::make_pair(end.first + removePos.first, end.second + removePos.second);
        Piece* newPiece = board->getPiece(removePosition);
        if(!newPiece) {
            return false;
        }
        removedPiece = newPiece;
        return board->removePiece(removePosition);
    }

    // See RemovePieceAction.h
    bool RemovePieceAction::reverseAction(GameBoard* board)
    {
        if(!board) {
            return false;
        }
        return board->unRemovePiece(removedPiece);
    }
}