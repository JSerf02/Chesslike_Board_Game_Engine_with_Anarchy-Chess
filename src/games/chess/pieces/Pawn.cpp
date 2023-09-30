#include <vector>
#include <memory>

#include "ChessBoard.h"
#include "ChessGameState.h"
#include "ChessPiece.h"
#include "Pawn.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knook.h"
#include "CapturePieceAction.h"
#include "TryCapturePieceAction.h"
#include "RemovePieceAction.h"
#include "AddPieceAction.h"

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
        addPosition(oneAhead, moves, chessState, 1, {});

        // Add a move two positions ahead if it is unoccupied and on the board
        // - Give this move a callback that sets this piece's boost turn
        Move::position twoAhead = std::make_pair(curPosition.first, curPosition.second + 2 * direction);
        if(!(board->unoccupiedOnBoard(twoAhead)) || !canBoost(chessState)) {
            return;
        }
        addPosition(twoAhead, moves, chessState, 1, {}, {
            make_action(new PawnBoostAction(*this, chessState))
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
        ChessBoard* chessBoard = static_cast<ChessBoard*>(board);
        Move::position curPosition = getPosition();
        Player curPlayer = getPlayer();
        int direction = getPlayerAccess(Player::white) ? 1 : -1;

        // The positions to check for enemies
        std::vector<Move::position> enemyDeltas {
            std::make_pair(1, 0),
            std::make_pair(-1, 0)
        };

        for(Move::position enemyDelta : enemyDeltas) {
            Move::position startPosition = std::make_pair(enemyDelta.first + curPosition.first, curPosition.second);
            Move::position checkPosition = std::make_pair(startPosition.first, startPosition.second);

            // Make sure there's a piece on the board
            if(!board->occupiedOnBoard(checkPosition)) {
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

            // Add the position
            if(attackOnly) {
                std::vector<Move> tempMoves{};
                bool addResult = addPosition(std::make_pair(checkPosition.first, checkPosition.second + direction), moves, chessState, 10, {
                    make_action(new EnPassantCaptureAction(startPosition, checkPosition))
                });
                if(!addResult) {
                    return;
                }
                addPosition(checkPosition, moves, chessState, 10);
            }
            else {
                bool addResult = addPosition(std::make_pair(checkPosition.first, checkPosition.second + direction), moves, chessState, 10, {
                    make_action(new EnPassantCaptureAction(startPosition, checkPosition))
                });
                if(!addResult) {
                    return;
                }
            }

            // Add long passant positions
            checkPosition = std::make_pair(checkPosition.first + enemyDelta.first, checkPosition.second + direction);
            while(board->occupiedOnBoard(checkPosition) && board->unoccupiedOnBoard(std::make_pair(checkPosition.first, checkPosition.second + direction))) {
                Piece* piece = board->getPiece(checkPosition);

                // Make sure the piece is a pawn
                if(piece->getID() != PAWN_ID || piece->getPlayerAccess(curPlayer)) {
                    continue;
                }
                
                // Add the position
                if(attackOnly) {
                    std::vector<Move> tempMoves{};
                    bool addResult = addPosition(std::make_pair(checkPosition.first, checkPosition.second + direction), moves, chessState, 10, {
                        make_action(new EnPassantCaptureAction(startPosition, checkPosition))
                    });
                    if(!addResult) {
                        return;
                    }
                    addPosition(checkPosition, moves, chessState, 10);
                }
                else {
                    bool addResult = addPosition(std::make_pair(checkPosition.first, checkPosition.second + direction), moves, chessState, 10, {
                        make_action(new EnPassantCaptureAction(startPosition, checkPosition))
                    });
                    if(!addResult) {
                        return;
                    }
                }

                // Add promotion and knight boosting
                if(checkPosition.second == (direction == 1 ? chessBoard->maxY() : chessBoard->minY()) - direction) {
                    Move::position promotionPosition = std::make_pair(checkPosition.first, checkPosition.second + direction);
                    if(!attackOnly) {
                        forceAddPromotion(moves, chessState, promotionPosition, 10, {
                            make_action(new EnPassantCaptureAction(startPosition, checkPosition))
                        });
                    }
                    forceAddKnightBoost(moves, chessState, promotionPosition, 10, {
                        make_action(new EnPassantCaptureAction(startPosition, checkPosition))
                    });
                    break;
                }
                checkPosition = std::make_pair(checkPosition.first + enemyDelta.first, checkPosition.second + direction);
            }
        }
    }

    // See Pawn.h
    void Pawn::addPromotion(std::vector<Move>& moves, ChessGameState& chessState)
    {
        // Store values for later use
        ChessBoard* board = static_cast<ChessBoard*>(chessState.getBoard());
        Move::position curPosition = getPosition();
        int direction = getPlayerAccess(Player::white) ? 1 : -1;
        Player player = direction == 1 ? Player::white : Player::black;

        // Add a move one position ahead if it is unoccupied and on the board
        Move::position oneAhead = std::make_pair(curPosition.first, curPosition.second + direction);
        if(!(board->unoccupiedOnBoard(oneAhead))) {
            return;
        }

        // Don't add promotion if the next tile is not the end of the board
        if(oneAhead.second != board->minY() && oneAhead.second != board->maxY()) {
            return;
        }

        // Add the promotion moves
        forceAddPromotion(moves, chessState, oneAhead);
    }

    // See Pawn.h
    void Pawn::forceAddPromotion(std::vector<Move>& moves, ChessGameState& chessState, Move::position promotePosition, int priority, std::vector<std::shared_ptr<Action>> extraPreMoves)
    {
        for(int i = static_cast<int>(PromotionIdx::pawn) + 1; i < static_cast<int>(PromotionIdx::length); i++) {
            addPosition(promotePosition, moves, chessState, priority, extraPreMoves, {
                make_action(new RemovePieceAction()),
                make_action(new AddPieceAction([i](std::vector<Player> players, Move::position start) {
                    switch(static_cast<PromotionIdx>(i)) {
                        case PromotionIdx::queen:
                            return static_cast<Piece*>(new Queen(players, start));
                        case PromotionIdx::knight:
                            return static_cast<Piece*>(new Knight(players, start));
                        case PromotionIdx::rook:
                            return static_cast<Piece*>(new Rook(players, start));
                        case PromotionIdx::bishop:
                            return static_cast<Piece*>(new Bishop(players, start));
                        case PromotionIdx::knook:
                            return static_cast<Piece*>(new Knook(players, start));
                    }
                    return static_cast<Piece*>(nullptr);
                }))
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

        // Add the knight boost moves
        forceAddKnightBoost(moves, chessState, oneAhead);
    }

    // See Pawn.h
    void Pawn::forceAddKnightBoost(std::vector<Move>& moves, ChessGameState& chessState, Move::position boostPosition, int priority, std::vector<std::shared_ptr<Action>> extraPreMoves)
    {
        // All possible knight moves
        const std::vector<Move::position> positions {
            std::make_pair( 1 + boostPosition.first,  2 + boostPosition.second),
            std::make_pair( 2 + boostPosition.first,  1 + boostPosition.second),
            std::make_pair( 2 + boostPosition.first, -1 + boostPosition.second),
            std::make_pair( 1 + boostPosition.first, -2 + boostPosition.second),
            std::make_pair(-1 + boostPosition.first, -2 + boostPosition.second),
            std::make_pair(-2 + boostPosition.first, -1 + boostPosition.second),
            std::make_pair(-2 + boostPosition.first,  1 + boostPosition.second),
            std::make_pair(-1 + boostPosition.first,  2 + boostPosition.second)
        };
        std::vector<std::shared_ptr<Action>> preMoves{};
        for(std::shared_ptr<Action> action : extraPreMoves) {
            preMoves.push_back(action);
        }
        preMoves.push_back(make_action(new TryCapturePieceAction(getPlayer())));
        addUnrelatedPositions(positions, moves, chessState, priority, preMoves, {
            make_action(new RemovePieceAction()),
            make_action(new AddPieceAction([](std::vector<Player> players, Move::position start){ return new Knight(players, start); }))
        });
    }

    // See Pawn.h
    void PawnBoostAction::applySymptomaticEffects(GameBoard* board)
    {
        pawn.setBoostTurn(gameState);
    }

    // See Pawn.h
    bool EnPassantCaptureAction::callAction(Move::position end, GameBoard* board)
    {
        // Do nothing if the board is invalid
        if(!board) {
            return false;
        }

        // Find the direction of the diagonal
        Move::position direction = std::make_pair(
            captureEnd.first >= captureStart.first ? 1 : -1,
            captureEnd.second >= captureStart.second ? 1 : -1
        );

        // Make sure each space on the diagonal is valid before removing anything
        Move::position curPosition = std::make_pair(captureStart.first, captureStart.second);
        while(true) {
            if(!board->occupiedOnBoard(curPosition)) {
                return false;
            }
            if(curPosition == captureEnd) {
                break;
            }
            curPosition = std::make_pair(curPosition.first + direction.first, curPosition.second + direction.second);
        }

        // Remove all of the pieces
        curPosition = std::make_pair(captureStart.first, captureStart.second);
        while(true) {
            removedPieces.push_back(board->getPiece(curPosition));
            board->removePiece(curPosition);
            if(curPosition == captureEnd) {
                break;
            }
            curPosition = std::make_pair(curPosition.first + direction.first, curPosition.second + direction.second);
        }
        return true;
    }

    bool EnPassantCaptureAction::reverseAction(GameBoard* board)
    {
        for(Piece* piece : removedPieces) {
            if(!board->unRemovePiece(piece)) {
                return false;
            }
        }
        removedPieces.clear();
        return true;
    }

    void EnPassantCaptureAction::applySymptomaticEffects(GameBoard* board)
    {
        for(Piece* piece : removedPieces) {
            board->captureRemovedPiece(piece);
        }
    }
}