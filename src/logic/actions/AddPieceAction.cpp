#include "Action.h"
#include "GameBoard.h"
#include "Move.h"
#include "AddPieceAction.h"

namespace logic {
    // See AddPieceAction.h
    bool AddPieceAction::callAction(Move::position end, GameBoard* board)
    {
        if(!board) {
            return false;
        }
        Move::position addPosition = std::make_pair(end.first + spawnPos.first, end.second + spawnPos.second);
        if(!board->unoccupiedOnBoard(addPosition)) {
            return false;
        }
        Piece* newPiece = pieceConstructor(players, std::make_pair(end.first + spawnPos.first, end.second + spawnPos.second));
        if(newPiece) {
            newPiece->validateMove();
        }
        return board->addPiece(newPiece);
    }

    // See AddPieceAction.h
    bool AddPieceAction::reverseAction(GameBoard* board)
    {
        if(!board) {
            return false;
        }
        return board->permanentlyRemoveMostRecentPiece();
    }
}