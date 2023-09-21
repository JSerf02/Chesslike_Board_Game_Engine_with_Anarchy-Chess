#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <unordered_map>

#include "GameState.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    /*
     * The standard 8x8 Chess board
    */
    class ChessBoard : public GameBoard
    {
        private:
            /* The length of each side of the board */
            const int boardSize = 8;
        
        public:
            /* Create a ChessBoard with all of the correct pieces */
            ChessBoard();
            
            /*
             * Returns true if the input is in the square chess board (all coordinates are in the interval [1, 8])
            */
            bool onBoard(int x, int y) override;

            /* 
            * Returns the minimum x value on the board 
            */
            int minX();

            /* 
            * Returns the maximum x value on the board
            */
            int maxX();

            /* 
            * Returns the minimum y value on the board
            */
            int minY();

            /*
            * Returns the maximum y value on the board
            */
            int maxY();
            
    };

}
#endif