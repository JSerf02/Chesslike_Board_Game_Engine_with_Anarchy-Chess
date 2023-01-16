#include "GameBoard.h"
#include "Piece.h"
#include "Move.h"

using Player = Piece::Player;

// See GameBoard.h
GameBoard::GameBoard(std::vector<Player> players)
{
    for(Player player : players) {
        capturedPieces[player] = new std::vector<Piece*>{};
    }
    setup();
}

//See GameBoard.h
GameBoard::~GameBoard() {
    // Free all captured piece vectors
    for(auto i = capturedPieces.begin(); i != capturedPieces.end(); i++) {
        auto playersCaptures = i->second;
        capturedPieces[i->first] = nullptr;
        delete playersCaptures;
    }

    // Free all pieces
    for (Piece*& piece: allPieces) {
        delete piece;
        piece = nullptr;
    }
}

// See GameBoard.h
bool GameBoard::onBoard(Move::position position) 
{
    return onBoard(position.first, position.second);
}

// See GameBoard.h
bool GameBoard::occupiedOnBoard(int x, int y) 
{
    return occupiedOnBoard(std::make_pair(x, y));
}

// See GameBoard.h
bool GameBoard::occupiedOnBoard(Move::position position)
{
    return onBoard(position) && (board[position] != nullptr);
}

// See GameBoard.h
bool GameBoard::unoccupiedOnBoard(int x, int y) 
{
    return unoccupiedOnBoard(std::make_pair(x, y));
}

// See GameBoard.h
bool GameBoard::unoccupiedOnBoard(Move::position position)
{
    return onBoard(position) && (board[position] == nullptr);
}

// See GameBoard.h
bool GameBoard::addPiece(Piece* piece)
{
    // Make sure a piece is inputted
    if(!piece) {
        return false;
    }

    // Store the position to prevent repeated calculations
    Move::position piecePosition = piece->getPosition();

    // Make sure the position is unoccupied
    if(!unoccupiedOnBoard(piecePosition)) {
        return false;
    }

    // Add the piece to the board and return true
    allPieces.push_back(piece);
    board[piecePosition] = piece;
    piece->setOnBoard(true);
    return true;
}

// See GameBoard.h
bool GameBoard::addPieces(std::vector<Piece*> pieces)
{
    for(Piece* piece : pieces) {
        if(!addPiece(piece)){
            return false;
        }
    }
    return true;
}

// See GameBoard.h
Piece* GameBoard::getPiece(int x, int y)
{
    return getPiece(std::make_pair(x, y));
}

// See GameBoard.h
Piece* GameBoard::getPiece(Move::position position)
{
    // Make sure the position is occupied, then return the piece
    if(!occupiedOnBoard(position)) {
        return nullptr;
    }
    return board[position];
}

// See GameBoard.h
bool GameBoard::removePiece(int x, int y)
{
    return removePiece(std::make_pair(x, y));
}

// See GameBoard.h
bool GameBoard::removePiece(Move::position position)
{
    // Make sure a piece is at the position before trying to remove
    Piece* piece = getPiece(position);
    if(!piece) {
        return false;
    }

    // Remove the piece
    piece->setOnBoard(false);
    board[position] = nullptr;
    return true;
}

// See GameBoard.h
bool GameBoard::capturePiece(int x, int y)
{
    return capturePiece(std::make_pair(x, y));
}

// See GameBoard.h
bool GameBoard::capturePiece(Move::position position)
{
    Piece *piece = getPiece(position);

    // Remove the piece and make sure that doesn't fail
    // Notice that removal not failing ensures piece is not null
    if(!removePiece(position)) {
        return false;
    }

    // Loop through all players' captured piece vectors and add the piece accordingly
    for(auto i = capturedPieces.begin(); i != capturedPieces.end(); i++) {
        if(!piece->getPlayerAccess(i->first)) { // Pieces are added for all players who don't control them
            i->second->push_back(piece);
        }
    }
    return true;
}

// See GameBoard.h
bool GameBoard::movePiece(int prevX, int prevY, int newX, int newY)
{
    return movePiece(std::make_pair(prevX, prevY), std::make_pair(newX, newY));
}

// See GameBoard.h
bool GameBoard::movePiece(int prevX, int prevY, Move::position newPosition) 
{
    return movePiece(std::make_pair(prevX, prevY), newPosition);
}

// See GameBoard.h
bool GameBoard::movePiece(Move::position prevPosition, int newX, int newY)
{
    return movePiece(prevPosition, std::make_pair(newX, newY));
}

// See GameBoard.h
bool GameBoard::movePiece(Move::position prevPosition, Move::position newPosition)
{
    // Make sure both spaces are on the board, the prev position is occupied, and
    // the new position is unoccupied
    if(!occupiedOnBoard(prevPosition) || !unoccupiedOnBoard(newPosition)) {
        return false;
    }

    // Remove the piece from the previous position and reinsert it at the new position
    Piece *piece = getPiece(prevPosition);
    board[prevPosition] = nullptr;
    board[newPosition] = piece;
    piece->changePosition(newPosition);
    return true;
}

// See GameBoard.h
std::vector<Move::position> GameBoard::getPiecesOfPlayer(Piece::Player player)
{
    std::vector<Move::position> piecePositions{};

    // Iterate through all of the pieces and add the ones that player can access
    for(Piece* piece : allPieces) {
        if(piece->getOnBoard() && piece->getPlayerAccess(player)) {
            piecePositions.push_back(piece->getPosition());
        }
    }

    return piecePositions;
}

// See GameBoard.h
const std::vector<Piece*>* GameBoard::getPlayerCaptures(Piece::Player player)
{
    // Make sure the game is tracking this player's pieces
    if(capturedPieces.find(player) == capturedPieces.end()) {
        return nullptr;
    }

    // Return the player's pieces
    return capturedPieces[player];
}

// See GameBoard.h
double GameBoard::getPlayerScore(Piece::Player player)
{
    // Get the player's captures and make sure nothing went wrong
    const std::vector<Piece*>* playerCaptures = getPlayerCaptures(player);
    if(playerCaptures == nullptr) {
        return 0;
    }

    // Add all of the scores of the player's captured pieces together
    double score = 0;
    for(Piece *piece : (*playerCaptures)) {
        score += piece->getValue();
    }
    return score;
}

//See GameBoard.h
void GameBoard::addToSimulation(SimulatedMove simulatedMove, Piece* piece, Move::position position)
{
    simulation.push_back({ simulatedMove, piece, position });
}

// See GameBoard.h
bool GameBoard::simulateMovePiece(Move::position prevPosition, Move::position newPosition)
{
    if(!movePiece(prevPosition, newPosition)) {
        return false;
    }
    Piece* piece = getPiece(newPosition);
    addToSimulation(SimulatedMove::movePiece, piece, prevPosition);
    return true;
}
bool GameBoard::simulateMovePiece(int prevX, int prevY, int newX, int newY)
{
    return simulateMovePiece(std::make_pair(prevX, prevY), std::make_pair(newX, newY));
}
bool GameBoard::simulateMovePiece(int prevX, int prevY, Move::position newPosition)
{
    return simulateMovePiece(std::make_pair(prevX, prevY), newPosition);
}
bool GameBoard::simulateMovePiece(Move::position prevPosition, int newX, int newY)
{
    return simulateMovePiece(prevPosition, std::make_pair(newX, newY));
}

// See GameBoard.h
bool GameBoard::simulateRemovePiece(int x, int y)
{
    return simulateRemovePiece(std::make_pair(x, y));
}
bool GameBoard::simulateRemovePiece(Move::position position)
{
    Piece* piece = getPiece(position);
    if(!piece || !removePiece(position)) {
        return false;
    }
    addToSimulation(SimulatedMove::removePiece, piece, position);
    return true;
}

// See GameBoard.h
bool GameBoard::simulateAddPiece(Piece* piece)
{
    if(!addPiece(piece)){
        return false;
    }
    addToSimulation(SimulatedMove::addPiece, piece, piece->getPosition());
    return true;
}

// See GameBoard.h
bool GameBoard::undoSimulatedMovePiece(Piece* piece, Move::position originalStart)
{
    return movePiece(piece->getPosition(), originalStart);
}

bool GameBoard::undoSimulatedRemovePiece(Piece* piece, Move::position originalPosition)
{
    if(!unoccupiedOnBoard(originalPosition)) {
        return false;
    }
    board[originalPosition] = piece;
    piece->setOnBoard(true);
    return true;
}

bool GameBoard::undoSimulatedAddPiece(Piece* piece, Move::position addedPosition)
{
    if(!removePiece(addedPosition)) {
        return false;
    }
    allPieces.pop_back();
    delete piece;
    return true;
}

// See GameBoard.h
bool GameBoard::revertSimulatedMove()
{
    SimulatedData moveData = simulation.back();
    bool result = false;
    switch(moveData.move){
        case SimulatedMove::movePiece:
            result = undoSimulatedMovePiece(moveData.piece, moveData.position);
            break;
        case SimulatedMove::removePiece:
            result = undoSimulatedRemovePiece(moveData.piece, moveData.position);
            break;
        case SimulatedMove::addPiece:
            result = undoSimulatedAddPiece(moveData.piece, moveData.position);
            break;
    }
    if(result) {
        simulation.pop_back();
    }
    return result;
}

// See GameBoard.h
bool GameBoard::revertSimulation()
{
    while(simulation.size() > 0) {
        if(!revertSimulatedMove()) {
            return false;
        }
    }
    return true;
}