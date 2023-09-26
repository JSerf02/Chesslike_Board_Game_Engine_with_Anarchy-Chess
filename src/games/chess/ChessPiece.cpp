#include <functional>
#include <memory>

#include "ChessPiece.h"
#include "ChessGameState.h"
#include "Piece.h"
#include "Move.h"
#include "Action.h"
#include "MovePieceAction.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;
    
    // See ChessPiece.h
    bool ChessPiece::isKing()
    {
        return getID() == KING_ID;
    }

    // See ChessPiece.h
    bool ChessPiece::addToMove(Move::position position, Move& move, ChessGameState& chessState)
    {    
        // If this piece is not controlled by the current player, bypass Checkmate checks
        // to prevent infinite loops
        if(!controlledByPlayer(chessState)) {
            move.addPosition(position);
            return true;
        }

        // Store values for later
        GameBoard* board = chessState.getBoard();
        Move::position curPosition = getPosition();

        // Create an action that, when called, will move the piece from start to end
        std::shared_ptr<Action> moveAction = make_action(new MovePieceAction(std::make_pair(position.first - curPosition.first, position.second - curPosition.second)));
        
        // Simulate all premove actions
        if(!chessState.callActions(move.getPreMoveActions(), position)) {
            return false;
        }

        // Simulate moving the piece
        if(!chessState.callAction(moveAction, curPosition)) {
            return false;
        }

        // Simulate all postmove actions
        if(!chessState.callActions(move.getPostMoveActions(), position)) {
            return false;
        }

        // Make sure the moves did not cause check
        bool isSafe = !chessState.isInCheck();
        board->revertSimulation();

        // Add the move only if it does not put the player in check
        if(isSafe) {
            move.addPosition(position);
        }
        return isSafe;
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addPosition(Move::position position, ChessGameState& chessState, int priority, std::vector<std::shared_ptr<Action>> preMoves, std::vector<std::shared_ptr<Action>> postMoves)
    {
        std::vector<Move> moves{};
        addPosition(position, moves, chessState, priority, preMoves, postMoves);
        return moves;
    }
    bool ChessPiece::addPosition(Move::position position, std::vector<Move>& moves, ChessGameState& chessState, int priority, std::vector<std::shared_ptr<Action>> preMoves, std::vector<std::shared_ptr<Action>> postMoves)
    {
        // Store values for later
        const Move::position curPosition = getPosition();
        GameBoard* board = chessState.getBoard();
        Player crntPlayer = getPlayer();

        // Cannot add if the space isn't on the board or is occupied
        if(!(board->onBoard(position))
        || (board->occupiedOnBoard(position) && board->getPiece(position)->getPlayerAccess(crntPlayer))) {
            return false;
        }
        
        // Try to create a new move with the inputted position
        Move newMove = Move({}, priority, preMoves, postMoves);
        if(addToMove(position, newMove, chessState)) {
            moves.push_back(newMove); // Add the new move if successful
            return true;
        }
        return false;
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addUnrelatedPositions(std::vector<Move::position> positions, ChessGameState& chessState, int priority, std::vector<std::shared_ptr<Action>> preMoves, std::vector<std::shared_ptr<Action>> postMoves)
    {
        std::vector<Move> moves{};
        addUnrelatedPositions(positions, moves, chessState, priority, preMoves, postMoves);
        return moves;
    }
    void ChessPiece::addUnrelatedPositions(std::vector<Move::position> positions, std::vector<Move>& moves, ChessGameState& chessState, int priority, std::vector<std::shared_ptr<Action>> preMoves, std::vector<std::shared_ptr<Action>> postMoves)
    {
        // Store values for later
        const Move::position curPosition = getPosition();
        GameBoard* board = chessState.getBoard();
        Player crntPlayer = getPlayer();

        // Iterate through all of the possible positions
        for(Move::position position : positions) {
            // Cannot add if the space isn't on the board or is occupied
            if(!(board->onBoard(position))
            || (board->occupiedOnBoard(position) && board->getPiece(position)->getPlayerAccess(crntPlayer))) {
                continue;
            }

            // Try to create a new move with the inputted position
            Move newMove = Move({}, priority, preMoves, postMoves);
            if(addToMove(position, newMove, chessState)) {
                moves.push_back(newMove); // Add the new move if successful
            }
        }
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addRelatedPositions(std::vector<Move::position> positions, ChessGameState& chessState, int priority, std::vector<std::shared_ptr<Action>> preMoves, std::vector<std::shared_ptr<Action>> postMoves)
    {
        std::vector<Move> moves{};
        addRelatedPositions(positions, moves, chessState, priority, preMoves, postMoves);
        return moves;
    }
    void ChessPiece::addRelatedPositions(std::vector<Move::position> positions, std::vector<Move>& moves, ChessGameState& chessState, int priority, std::vector<std::shared_ptr<Action>> preMoves, std::vector<std::shared_ptr<Action>> postMoves)
    {
        // Store values for later
        const Move::position curPosition = getPosition();
        GameBoard* board = chessState.getBoard();
        Player crntPlayer = getPlayer();

        // Create a new move to store the positions
        Move newMove = Move({}, priority, preMoves, postMoves);

        // Keeps track of whether a valid position has been detected
        // - Used to prevent adding empty moves to the moves list
        bool added = false;

        // Iterate through all of the possible positions
        for(Move::position position : positions) {
            // Cannot add if the space isn't on the board or is occupied
            if(!(board->onBoard(position))) {
                break;
            }

            // Occupied spaces can only be added if they are controlled by the enemy
            bool occupied = board->occupiedOnBoard(position);
            if (occupied && board->getPiece(position)->getPlayerAccess(crntPlayer)) {
                break;
            }
            
            // Add the position to the move
            if(addToMove(position, newMove, chessState)) {
                added = true;
            }
            else {
                break;
            }

            // Stop if the tile just added is occupied
            if(occupied) {
                break;
            }
        }

        // Add the new move if it contains at least one valid position
        if(added) {
            moves.push_back(newMove);
        }
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addUnrelatedPositionsDeltas(std::vector<Move::position> deltas, ChessGameState& chessState, int priority, std::vector<std::shared_ptr<Action>> preMoves, std::vector<std::shared_ptr<Action>> postMoves)
    {
        std::vector<Move> moves{};
        addUnrelatedPositionsDeltas(deltas, moves, chessState, priority, preMoves, postMoves);
        return moves;
    }
    void ChessPiece::addUnrelatedPositionsDeltas(std::vector<Move::position> deltas, std::vector<Move>& moves, ChessGameState& chessState, int priority, std::vector<std::shared_ptr<Action>> preMoves, std::vector<std::shared_ptr<Action>> postMoves)
    {
        // Store the current position
        Move::position curPosition = getPosition();

        // Create a vector to store the positions found from the deltas
        std::vector<Move::position> positions{};
        
        // Add a position for each delta equal to curPosition + delta
        for(Move::position delta : deltas) {
            positions.push_back(std::make_pair(curPosition.first + delta.first, curPosition.second + delta.second));
        }

        // Having positions reduces this function to addUnrelatedPositions
        addUnrelatedPositions(positions, moves, chessState, priority, preMoves, postMoves);
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addRelatedPositionsDeltas(std::vector<Move::position> deltas, ChessGameState& chessState, int priority, std::vector<std::shared_ptr<Action>> preMoves, std::vector<std::shared_ptr<Action>> postMoves)
    {
        std::vector<Move> moves{};
        addRelatedPositionsDeltas(deltas, moves, chessState, priority, preMoves, postMoves);
        return moves;
    }
    void ChessPiece::addRelatedPositionsDeltas(std::vector<Move::position> deltas, std::vector<Move>& moves, ChessGameState& chessState, int priority, std::vector<std::shared_ptr<Action>> preMoves, std::vector<std::shared_ptr<Action>> postMoves)
    {
        // Store the current position
        Move::position curPosition = getPosition();

        // Create a vector to store the positions found from the deltas
        std::vector<Move::position> positions{};
        
        // Add a position for each delta equal to curPosition + delta
        for(Move::position delta : deltas) {
            positions.push_back(std::make_pair(curPosition.first + delta.first, curPosition.second + delta.second));
        }

        // Having positions reduces this function to addRelatedPositions
        addRelatedPositions(positions, moves, chessState, priority, preMoves, postMoves);
    }

    // See ChessPiece.h
    Player ChessPiece::getPlayer() {
        return getPlayerAccess(Player::white) ? Player::white : Player::black;
    }
}