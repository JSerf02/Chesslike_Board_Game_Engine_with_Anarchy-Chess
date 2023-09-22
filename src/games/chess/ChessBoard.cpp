#include <unordered_map>

#include "ChessBoard.h"
#include "GameState.h"
#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    // See ChessBoard.h
    ChessBoard::ChessBoard(bool setup) : GameBoard({Player::white, Player::black}) 
    {
        // Don't add pieces if the setup flag is not enabled
        if(!setup) {
            return;
        }
        
        // White strong pieces
        Piece* whiteLeftRook    = new Rook  (Player::white, std::make_pair(1, 1));
        Piece* whiteLeftKnight  = new Knight(Player::white, std::make_pair(2, 1));
        Piece* whiteLeftBishop  = new Bishop(Player::white, std::make_pair(3, 1));
        Piece* whiteQueen       = new Queen (Player::white, std::make_pair(4, 1));
        Piece* whiteKing        = new King  (Player::white, std::make_pair(5, 1));
        Piece* whiteRightBishop = new Bishop(Player::white, std::make_pair(6, 1));
        Piece* whiteRightKnight = new Knight(Player::white, std::make_pair(7, 1));
        Piece* whiteRightRook   = new Rook  (Player::white, std::make_pair(8, 1));

        // Black strong pieces
        Piece* blackLeftRook    = new Rook  (Player::black, std::make_pair(1, 8));
        Piece* blackLeftKnight  = new Knight(Player::black, std::make_pair(2, 8));
        Piece* blackLeftBishop  = new Bishop(Player::black, std::make_pair(3, 8));
        Piece* blackQueen       = new Queen (Player::black, std::make_pair(4, 8));
        Piece* blackKing        = new King  (Player::black, std::make_pair(5, 8));
        Piece* blackRightBishop = new Bishop(Player::black, std::make_pair(6, 8));
        Piece* blackRightKnight = new Knight(Player::black, std::make_pair(7, 8));
        Piece* blackRightRook   = new Rook  (Player::black, std::make_pair(8, 8));

        // Add all of the strong pieces to the board
        addPieces({
            whiteLeftRook,
            whiteLeftKnight,
            whiteLeftBishop,
            whiteQueen,
            whiteKing,
            whiteRightBishop,
            whiteRightKnight,
            whiteRightRook,
            blackLeftRook,
            blackLeftKnight,
            blackLeftBishop,
            blackQueen,
            blackKing,
            blackRightBishop,
            blackRightKnight,
            blackRightRook
        });

        // Add all of the pawns to the board
        for (int col = 1; col <= 8; col++) {
            addPiece(new Pawn(Player::white, std::make_pair(col, 2)));
            addPiece(new Pawn(Player::black, std::make_pair(col, 7)));
        }
    }

    // See ChessBoard.h
    bool ChessBoard::onBoard(int x, int y) 
    {
        return 1 <= x  && x <= 8 && 1 <= y && y <= 8;
    }

    // See ChessBoard.h
    int ChessBoard::minX() 
    {
        return 1;
    }

    // See ChessBoard.h
    int ChessBoard::maxX() 
    {
        return boardSize;
    }

    // See ChessBoard.h
    int ChessBoard::minY() 
    {
        return 1;
    }

    // See ChessBoard.h
    int ChessBoard::maxY() 
    {
        return boardSize;
    }

}