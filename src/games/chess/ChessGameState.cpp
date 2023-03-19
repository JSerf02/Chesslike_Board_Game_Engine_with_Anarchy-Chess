#include <unordered_map>

#include "ChessGameState.h"
#include "GameState.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;


    // See ChessGameState.h
    ChessGameState::ChessGameState()
    {
        return;
    }

    // See ChessGameState.h
    Piece* ChessGameState::getKing(Player player) {
        return kings[player];
    }

    // See ChessGameState.h
    bool ChessGameState::isInCheck(Player player)
    {
        return true;
    }
    bool ChessGameState::isInCheck()
    {
        return true;
    }

    // See ChessGameState.h
    bool ChessGameState::willMoveCauseCheck(int startX, int startY, int endX, int endY)
    {
        return true;
    }
    bool ChessGameState::willMoveCauseCheck(Move::position start, int endX, int endY)
    {
        return true;
    }
    bool ChessGameState::willMoveCauseCheck(int startX, int startY, Move::position end)
    {
        return true;
    }
    bool ChessGameState::willMoveCauseCheck(Move::position start, Move::position end)
    {
        return true;
    }

    // See ChessGameState.h
    bool ChessGameState::isInCheckmate(Player player)
    {
        return true;
    }
    bool ChessGameState::isInCheckmate()
    {
        return true;
    }

}