#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Knight.h"
#include "RemovePieceAction.h"
#include "AddPieceAction.h"
#include "Knook.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;
    using ID = Piece::ID;

    // See Knight.h
    double Knight::getValue() 
    {
        return 3.0;
    }

    // See Knight.h
    ID Knight::getID()
    {
        return KNIGHT_ID;
    }

    // See Knight.h
    std::vector<Move> Knight::generateMoves(GameState& gameState)
    {
        std::vector<Move> moves = generateStandardMoves(gameState);
        addKnooklearFusion(moves, gameState);
        return moves;
    }

    // See Knight.h
    std::vector<Move> Knight::generateAttackingMoves(GameState& gameState)
    {
        return generateStandardMoves(gameState);
    }

    // See Knight.h
    std::vector<Move> Knight::generateStandardMoves(GameState& gameState)
    {
        // All of the different L positions
        const std::vector<Move::position> deltas {
            std::make_pair(1, 2),
            std::make_pair(2, 1),
            std::make_pair(2, -1),
            std::make_pair(1, -2),
            std::make_pair(-1, -2),
            std::make_pair(-2, -1),
            std::make_pair(-2, 1),
            std::make_pair(-1, 2)
        };
        return addUnrelatedPositionsDeltas(deltas, static_cast<ChessGameState&>(gameState));
    }

    // See Knight.h
    void Knight::addKnooklearFusion(std::vector<Move>& moves, GameState& gameState)
    {
        // All of the different L positions
        const std::vector<Move::position> deltas {
            std::make_pair(1, 2),
            std::make_pair(2, 1),
            std::make_pair(2, -1),
            std::make_pair(1, -2),
            std::make_pair(-1, -2),
            std::make_pair(-2, -1),
            std::make_pair(-2, 1),
            std::make_pair(-1, 2)
        };
        Move::position curPosition = getPosition();
        GameBoard* board = gameState.getBoard();
        Player player = getPlayer();
        std::vector<Move::position> fusionPositions {};
        for(Move::position delta : deltas) {
            Move::position targetPosition = std::make_pair(curPosition.first + delta.first, curPosition.second + delta.second);
            if(!board->occupiedOnBoard(targetPosition)) {
                continue;
            }
            Piece* piece = board->getPiece(targetPosition);
            if(!piece->getPlayerAccess(player) || piece->getID() != ROOK_ID) {
                continue;
            }
            fusionPositions.push_back(targetPosition);
        }

        addUnrelatedPositions(fusionPositions, moves, static_cast<ChessGameState&>(gameState), 1, {
            make_action(new RemovePieceAction())
        }, {
            make_action(new RemovePieceAction()),
            make_action(new AddPieceAction(player, [](std::vector<Player> players, Move::position start){ return new Knook(players, start); }))
        });

    }
}