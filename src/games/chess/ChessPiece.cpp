#include "ChessPiece.h"
#include "ChessGameState.h"
#include "Piece.h"
#include "Move.h"

#include <functional>

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

        // Get a reference to the game board
        GameBoard* board = chessState.getBoard();
        
        // Call the onMoveCallback with simulation enabled to simulate everything 
        // that's done before moving the piece
        board->clearFutureMoves(); // Clear the future moves list so excess moves do not affect this one
        move.callOnMove(getPosition(), position, chessState, true);

        // Simulate moving the piece and see if it will put the player in check
        if(board->simulateMovePiece(getPosition(), position) == false) {
            board->revertSimulation();
            return false; // The move is impossible so it cannot be added
        }

        // Apply the future moves to allow for unusual beahvior, such as piece swapping
        board->simulateApplyFutureMoves();

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
    std::vector<Move> ChessPiece::addPosition(Move::position position, ChessGameState& chessState, int priority, std::function<void (Move::position, Move::position, GameState&, bool)> onMoveCallback)
    {
        std::vector<Move> moves{};
        addPosition(position, moves, chessState, priority, onMoveCallback);
        return moves;
    }
    bool ChessPiece::addPosition(Move::position position, std::vector<Move>& moves, ChessGameState& chessState, int priority, std::function<void (Move::position, Move::position, GameState&, bool)> onMoveCallback)
    {
        // Store values for later
        const Move::position curPosition = getPosition();
        GameBoard* board = chessState.getBoard();
        Player crntPlayer = getPlayerAccess(Player::white) ? Player::white : Player::black;

        // Cannot add if the space isn't on the board or is occupied
        if(!(board->onBoard(position))
        || (board->occupiedOnBoard(position) && board->getPiece(position)->getPlayerAccess(crntPlayer))) {
            return false;
        }
        
        // Try to create a new move with the inputted position
        Move newMove = Move({}, priority, onMoveCallback);
        if(addToMove(position, newMove, chessState)) {
            moves.push_back(newMove); // Add the new move if successful
            return true;
        }
        return false;
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addUnrelatedPositions(std::vector<Move::position> positions, ChessGameState& chessState, int priority, std::function<void (Move::position, Move::position, GameState&, bool)> onMoveCallback)
    {
        std::vector<Move> moves{};
        addUnrelatedPositions(positions, moves, chessState, priority, onMoveCallback);
        return moves;
    }
    void ChessPiece::addUnrelatedPositions(std::vector<Move::position> positions, std::vector<Move>& moves, ChessGameState& chessState, int priority, std::function<void (Move::position, Move::position, GameState&, bool)> onMoveCallback)
    {
        // Store values for later
        const Move::position curPosition = getPosition();
        GameBoard* board = chessState.getBoard();
        Player crntPlayer = getPlayerAccess(Player::white) ? Player::white : Player::black;

        // Iterate through all of the possible positions
        for(Move::position position : positions) {
            // Cannot add if the space isn't on the board or is occupied
            if(!(board->onBoard(position))
            || (board->occupiedOnBoard(position) && board->getPiece(position)->getPlayerAccess(crntPlayer))) {
                continue;
            }

            // Try to create a new move with the inputted position
            Move newMove = Move({}, priority, onMoveCallback);
            if(addToMove(position, newMove, chessState)) {
                moves.push_back(newMove); // Add the new move if successful
            }
        }
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addRelatedPositions(std::vector<Move::position> positions, ChessGameState& chessState, int priority, std::function<void (Move::position, Move::position, GameState&, bool)> onMoveCallback)
    {
        std::vector<Move> moves{};
        addRelatedPositions(positions, moves, chessState, priority, onMoveCallback);
        return moves;
    }
    void ChessPiece::addRelatedPositions(std::vector<Move::position> positions, std::vector<Move>& moves, ChessGameState& chessState, int priority, std::function<void (Move::position, Move::position, GameState&, bool)> onMoveCallback)
    {
        // Store values for later
        const Move::position curPosition = getPosition();
        GameBoard* board = chessState.getBoard();
        Player crntPlayer = getPlayerAccess(Player::white) ? Player::white : Player::black;

        // Create a new move to store the positions
        Move newMove = Move({}, priority, onMoveCallback);

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
    std::vector<Move> ChessPiece::addUnrelatedPositionsDeltas(std::vector<Move::position> deltas, ChessGameState& chessState, int priority, std::function<void (Move::position, Move::position, GameState&, bool)> onMoveCallback)
    {
        std::vector<Move> moves{};
        addUnrelatedPositionsDeltas(deltas, moves, chessState, priority, onMoveCallback);
        return moves;
    }
    void ChessPiece::addUnrelatedPositionsDeltas(std::vector<Move::position> deltas, std::vector<Move>& moves, ChessGameState& chessState, int priority, std::function<void (Move::position, Move::position, GameState&, bool)> onMoveCallback)
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
        addUnrelatedPositions(positions, moves, chessState, priority, onMoveCallback);
    }

    // See ChessPiece.h
    std::vector<Move> ChessPiece::addRelatedPositionsDeltas(std::vector<Move::position> deltas, ChessGameState& chessState, int priority, std::function<void (Move::position, Move::position, GameState&, bool)> onMoveCallback)
    {
        std::vector<Move> moves{};
        addRelatedPositionsDeltas(deltas, moves, chessState, priority, onMoveCallback);
        return moves;
    }
    void ChessPiece::addRelatedPositionsDeltas(std::vector<Move::position> deltas, std::vector<Move>& moves, ChessGameState& chessState, int priority, std::function<void (Move::position, Move::position, GameState&, bool)> onMoveCallback)
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
        addRelatedPositions(positions, moves, chessState, priority, onMoveCallback);
    }

    void ChessPiece::captureCallback(Move::position start, Move::position end, GameState& gameState, bool simulating)
    {
        // Do not capture if there is no piece to capture
        GameBoard* board = gameState.getBoard();
        if(board->occupiedOnBoard(end) == false) {
            return;
        }

        // Get the current player
        Piece* curPiece = board->getPiece(start);
        Player curPlayer = curPiece->getPlayerAccess(Player::white) ? Player::white : Player::black;
        
        // Do not capture if the piece is controlled by the current player
        Piece* piece = board->getPiece(end);
        if(piece->getPlayerAccess(curPlayer)) {
            return;
        }
        
        // Capture the piece
        if(simulating) {
            board->simulateRemovePiece(end);
        }
        else {
            board->capturePiece(end);
        }
    }
}