#include "GameBoard.h"
#include "Piece.h"
#include "Move.h"

// See GameBoard.h
GameBoard::GameBoard()
{
    setup();
}

// See GameBoard.h
bool GameBoard::onBoard(Move::position position) 
{
    return onBoard(position.first, position.second);
}

// See GameBoard.h
bool GameBoard::occupiedOnBoard(int x, int y) 
{
    return true;
}

// See GameBoard.h
bool GameBoard::occupiedOnBoard(Move::position position)
{
    return occupiedOnBoard(position.first, position.second);
}

// See GameBoard.h
bool GameBoard::addPiece(Piece* piece)
{
    return true;
}

// See GameBoard.h
Piece* GameBoard::getPiece(int x, int y)
{
    return nullptr;
}

// See GameBoard.h
Piece* GameBoard::getPiece(Move::position position)
{
    return getPiece(position.first, position.second);
}

// See GameBoard.h
bool GameBoard::removePiece(int x, int y)
{
    return true;
}

// See GameBoard.h
bool GameBoard::removePiece(Move::position position)
{
    return removePiece(position.first, position.second);
}

// See GameBoard.h
bool GameBoard::capturePiece(int x, int y)
{
    return true;
}

// See GameBoard.h
bool GameBoard::capturePiece(Move::position position)
{
    return capturePiece(position.first, position.second);
}

// See GameBoard.h
bool GameBoard::movePiece(int prevX, int prevY, int newX, int newY)
{
    return true;
}

// See GameBoard.h
bool GameBoard::movePiece(int prevX, int prevY, Move::position newPosition) 
{
    return movePiece(prevX, prevY, newPosition.first, newPosition.second);
}

// See GameBoard.h
bool GameBoard::movePiece(Move::position prevPosition, int newX, int newY)
{
    return movePiece(prevPosition.first, prevPosition.second, newX, newY);
}

// See GameBoard.h
bool GameBoard::movePiece(Move::position prevPosition, Move::position newPosition)
{
    return movePiece(prevPosition.first, prevPosition.second, newPosition.first, 
        newPosition.second);
}

// See GameBoard.h
const std::vector<Piece*>& GameBoard::getPlayerCaptures(Piece::Player player)
{
    return {};
}

// See GameBoard.h
int GameBoard::getPlayerScore(Piece::Player player)
{
    return 0;
}