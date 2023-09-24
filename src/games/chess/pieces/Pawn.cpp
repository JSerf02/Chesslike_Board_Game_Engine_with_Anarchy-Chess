#include <vector>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Pawn.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;
    using ID = Piece::ID;

    // See Pawn.h
    double Pawn::getValue() 
    {
        return 1.0;
    }

    // See Pawn.h
    ID Pawn::getID()
    {
        return PAWN_ID;
    }

    // See Pawn.h
    bool Pawn::canBoost(GameState& gameState)
    {
        Player player = getPlayerAccess(Player::white) ? Player::white : Player::black;
        ChessBoard* board = static_cast<ChessBoard*>(gameState.getBoard());
        int startY = (player == Player::white) ? board->minY() + 1 : board->maxY() - 1;
        return !previouslyMoved() && getPosition().second == startY;
    }

    // See Pawn.h
    void Pawn::setBoostTurn(GameState& gameState)
    {
        boostTurn = gameState.getTurn();
    }

    // See Pawn.h
    int Pawn::getMinPriority(GameState& gameState)
    {
        // Store values for later use
        GameBoard* board = gameState.getBoard();
        Move::position curPosition = getPosition();
        Player curPlayer = getPlayerAccess(Player::white) ? Player::white : Player::black;
        int direction = getPlayerAccess(Player::white) ? 1 : -1;

        // The positions to check for enemies
        std::vector<Move::position> enemyDeltas {
            std::make_pair(1, 0),
            std::make_pair(-1, 0)
        };

        // Override the priority if En Passant is a possibility
        for(Move::position enemyDelta : enemyDeltas) {
            Move::position checkPosition = std::make_pair(enemyDelta.first + curPosition.first, curPosition.second);

            // Make sure there's a piece on the board
            if(!(board->occupiedOnBoard(checkPosition))) {
                continue;
            }
            Piece* piece = board->getPiece(checkPosition);

            // Make sure the piece is a pawn
            if(piece->getID() != PAWN_ID || piece->getPlayerAccess(curPlayer)) {
                continue;
            }
            Pawn* pawn = static_cast<Pawn*>(piece); // Will always work as long as IDs are configured correctly

            // Make sure the pawn just boosted
            if(!(pawn->justBoosted(gameState))) {
                continue;
            }

            // If all previous conditions hold, En Passant is a possibility and 
            // the priority needs to be overridden
            return 10;
        }
        return 0;
    }

    // See Pawn.h
    std::vector<Move> Pawn::generateMoves(GameState& gameState)
    {
        std::vector<Move> moves{};
        ChessGameState& chessState = static_cast<ChessGameState&>(gameState);
        addForwardMoves(moves, chessState);
        addStandardAttacks(moves, chessState);
        addEnPassant(moves, chessState);
        addPromotion(moves, chessState);
        addKnightBoost(moves, chessState);
        return moves;
    }

    // See Pawn.h
    std::vector<Move> Pawn::generateAttackingMoves(GameState& gameState)
    {
        std::vector<Move> moves{};
        ChessGameState& chessState = static_cast<ChessGameState&>(gameState);
        addStandardAttacks(moves, chessState, true);
        addEnPassant(moves, chessState, true);
        addKnightBoost(moves, chessState);
        return moves;
    }

    // See Pawn.h
    bool Pawn::justBoosted(GameState& gameState)
    {
        return boostTurn == gameState.getTurn() - 1;
    }

    // See Pawn.h
    void Pawn::addForwardMoves(std::vector<Move>& moves, ChessGameState& chessState)
    {
        // Store values for later use
        GameBoard* board = chessState.getBoard();
        Move::position curPosition = getPosition();
        int direction = getPlayerAccess(Player::white) ? 1 : -1;

        // Add a move one position ahead if it is unoccupied and on the board
        Move::position oneAhead = std::make_pair(curPosition.first, curPosition.second + direction);
        if(!(board->unoccupiedOnBoard(oneAhead))) {
            return;
        }
        addPosition(oneAhead, moves, chessState, 1, nullptr);

        // Add a move two positions ahead if it is unoccupied and on the board
        // - Give this move a callback that sets this piece's boost turn
        Move::position twoAhead = std::make_pair(curPosition.first, curPosition.second + 2 * direction);
        if(!(board->unoccupiedOnBoard(twoAhead)) || !canBoost(chessState)) {
            return;
        }
        addPosition(twoAhead, moves, chessState, 1, 
        [](Move::position start, Move::position end, GameState& gameState, bool simulation) {
            if(simulation) {
                return;
            }
            Pawn* pawn = static_cast<Pawn*>(gameState.getBoard()->getPiece(start));
            pawn->setBoostTurn(gameState);
        });
    }

    // See Pawn.h
    void Pawn::addStandardAttacks(std::vector<Move>& moves, ChessGameState& chessState, bool attackOnly)
    {
        // Store values for later use
        GameBoard* board = chessState.getBoard();
        Move::position curPosition = getPosition();
        Player curPlayer = getPlayerAccess(Player::white) ? Player::white : Player::black;
        int direction = getPlayerAccess(Player::white) ? 1 : -1;

        // All of the possible attack deltas
        std::vector<Move::position> deltas {
            std::make_pair(1, direction),
            std::make_pair(-1, direction)
        };

        // Add positions based on deltas if this is attack only or if the position
        // contains an enemy piece
        std::vector<Move::position> positions{};
        for(Move::position delta : deltas) {
            Move::position newPosition = std::make_pair(curPosition.first + delta.first, curPosition.second + delta.second);
            if(attackOnly
            || (board->occupiedOnBoard(newPosition) && !(board->getPiece(newPosition)->getPlayerAccess(curPlayer)))) {
                positions.push_back(newPosition);
            }
        }
        addUnrelatedPositions(positions, moves, chessState);
    }

    // See Pawn.h
    void Pawn::addEnPassant(std::vector<Move>& moves, ChessGameState& chessState, bool attackOnly)
    {
        // Store values for later use
        GameBoard* board = chessState.getBoard();
        Move::position curPosition = getPosition();
        Player curPlayer = getPlayerAccess(Player::white) ? Player::white : Player::black;
        int direction = getPlayerAccess(Player::white) ? 1 : -1;

        // The positions to check for enemies
        std::vector<Move::position> enemyDeltas {
            std::make_pair(1, 0),
            std::make_pair(-1, 0)
        };

        // Add deltas based on each position that contains a pawn that just boosted
        std::vector<Move::position> deltas{};
        for(Move::position enemyDelta : enemyDeltas) {
            Move::position checkPosition = std::make_pair(enemyDelta.first + curPosition.first, curPosition.second);

            // Make sure there's a piece on the board
            if(!(board->occupiedOnBoard(checkPosition))) {
                continue;
            }
            Piece* piece = board->getPiece(checkPosition);

            // Make sure the piece is a pawn
            if(piece->getID() != PAWN_ID || piece->getPlayerAccess(curPlayer)) {
                continue;
            }
            Pawn* pawn = static_cast<Pawn*>(piece); // Will always work as long as IDs are configured correctly

            // Make sure the pawn just boosted
            if(!(pawn->justBoosted(chessState))) {
                continue;
            }

            // If all previous conditions hold, add the En Passant delta
            deltas.push_back(std::make_pair(enemyDelta.first, attackOnly ? 0 : direction));
        }

        addUnrelatedPositionsDeltas(deltas, moves, chessState, 10,
        [](Move::position start, Move::position end, GameState& gameState, bool simulation) {
            GameBoard* board = gameState.getBoard();
            if(simulation) {
                board->simulateRemovePiece(end.first, start.second);
            }
            else {
                board->capturePiece(end.first, start.second);
            }
        });
    }

    // See Pawn.h
    void Pawn::addPromotion(std::vector<Move>& moves, ChessGameState& chessState)
    {
        // Store values for later use
        ChessBoard* board = static_cast<ChessBoard*>(chessState.getBoard());
        Move::position curPosition = getPosition();
        int direction = getPlayerAccess(Player::white) ? 1 : -1;

        // Add a move one position ahead if it is unoccupied and on the board
        Move::position oneAhead = std::make_pair(curPosition.first, curPosition.second + direction);
        if(!(board->unoccupiedOnBoard(oneAhead))) {
            return;
        }

        // Don't add promotion if the next tile is not the end of the board
        if(oneAhead.second != board->minY() && oneAhead.second != board->maxY()) {
            return;
        }

        for(int i = static_cast<int>(PromotionIdx::pawn) + 1; i < static_cast<int>(PromotionIdx::length); i++) {
            addPosition(oneAhead, moves, chessState, 1, 
            [i](Move::position start, Move::position end, GameState& gameState, bool simulation) {
                // Get values for later
                GameBoard* board = gameState.getBoard();
                Piece* pawn = board->getPiece(start);
                Player player = pawn->getPlayerAccess(Player::white) ? Player::white : Player::black;

                // Remove the pawn
                if(simulation) {
                    board->simulateRemovePiece(start);
                }
                else {
                    board->removePiece(start);
                }

                // Figure out which piece to add
                Piece* newPiece = nullptr;
                switch(static_cast<PromotionIdx>(i)) {
                    case PromotionIdx::queen:
                        newPiece = new Queen(player, start);
                        break;
                    case PromotionIdx::knight:
                        newPiece = new Knight(player, start);
                        break;
                    case PromotionIdx::rook:
                        newPiece = new Rook(player, start);
                        break;
                    case PromotionIdx::bishop:
                        newPiece = new Bishop(player, start);
                        break;
                }
                if(!newPiece) {
                    return;
                }

                // Make sure the newPiece doesn't trigger any starting position conditions
                newPiece->validateMove();

                // Add the new piece at the pawn's current position
                // - It will move forward one space afterwards
                if(simulation) {
                    board->simulateAddPiece(newPiece);
                }
                else {
                    board->addPiece(newPiece);
                }
            });
        }
    }

    // See Pawn.h
    void Pawn::addKnightBoost(std::vector<Move>& moves, ChessGameState& chessState)
    {   
        // Store values for later use
        ChessBoard* board = static_cast<ChessBoard*>(chessState.getBoard());
        Move::position curPosition = getPosition();
        int direction = getPlayerAccess(Player::white) ? 1 : -1;

        // Add a move one position ahead if it is unoccupied and on the board
        Move::position oneAhead = std::make_pair(curPosition.first, curPosition.second + direction);
        if(!(board->unoccupiedOnBoard(oneAhead))) {
            return;
        }

        // Don't add promotion if the next tile is not the end of the board
        if(oneAhead.second != board->minY() && oneAhead.second != board->maxY()) {
            return;
        }

        // All possible knight moves
        const std::vector<Move::position> deltas {
            std::make_pair( 1,  2 + direction),
            std::make_pair( 2,  1 + direction),
            std::make_pair( 2, -1 + direction),
            std::make_pair( 1, -2 + direction),
            std::make_pair(-1, -2 + direction),
            std::make_pair(-2, -1 + direction),
            std::make_pair(-2,  1 + direction),
            std::make_pair(-1,  2 + direction)
        };
        addUnrelatedPositionsDeltas(deltas, moves, chessState, 1, 
        [](Move::position start, Move::position end, GameState& gameState, bool simulating) {
            // Get values for later
            GameBoard* board = gameState.getBoard();
            Piece* pawn = board->getPiece(start);
            Player player = pawn->getPlayerAccess(Player::white) ? Player::white : Player::black;

            // Create the new knight
            Piece* newPiece = new Knight(player, start);

            // Make sure the newPiece doesn't trigger any starting position conditions
            newPiece->validateMove();

            // Replace the pawn with the knight
            if(simulating) {
                board->simulateRemovePiece(start);
                board->simulateAddPiece(newPiece);
            }
            else {
                board->removePiece(start);
                board->addPiece(newPiece);
            }
            captureCallback(start, end, gameState, simulating);
        });
    }
}