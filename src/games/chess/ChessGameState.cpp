#include <unordered_map>

#include "ChessGameState.h"
#include "GameState.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;
    
    // See ChessGameState.h
    ChessGameState::ChessGameState() : GameState(new ChessBoard(), {Player::white, Player::black}) {
        kings[Player::white] = getBoard()->getPiece(std::make_pair(5, 1));
        kings[Player::black] = getBoard()->getPiece(std::make_pair(5, 8));
    }

    // See ChessGameState.h
    Piece* ChessGameState::getKing(Player player) {
        return kings[player];
    }
    Piece* ChessGameState::getKing() {
        return getKing(getCrntPlayer());
    }

    // See ChessGameState.h
    bool ChessGameState::isInCheck(Player player)
    {
        return isAttacked(kings[player]->getPosition());
    }
    bool ChessGameState::isInCheck()
    {
        return isInCheck(getCrntPlayer());
    }

    // See ChessGameState.h
    bool ChessGameState::willMoveCauseCheck(int startX, int startY, int endX, int endY)
    {
        return willMoveCauseCheck(std::make_pair(startX, startY), std::make_pair(endX, endY));
    }
    bool ChessGameState::willMoveCauseCheck(Move::position start, int endX, int endY)
    {
        return willMoveCauseCheck(start, std::make_pair(endX, endY));
    }
    bool ChessGameState::willMoveCauseCheck(int startX, int startY, Move::position end)
    {
        return willMoveCauseCheck(std::make_pair(startX, startY), end);
    }
    bool ChessGameState::willMoveCauseCheck(Move::position start, Move::position end)
    {
        // Get a reference to the board
        GameBoard* board = getBoard();

        // Simulate moving the piece and check if the move will cause check
        if(!board->simulateMovePiece(start, end)) {
            return true; // Assume any invalid moves cause check
        }
        bool result = isInCheck();

        // Undo the simulated move and return the result
        board->revertSimulation();
        return result;
    }

    // See ChessGameState.h
    bool ChessGameState::isInCheckmate(Player player)
    {
        // Every move has priority of at least 1, so 0 priority means no possible moves
        return getPriorityOfPlayer(player) == 0; 
    }
    bool ChessGameState::isInCheckmate()
    {
        return isInCheckmate(getCrntPlayer());
    }

}