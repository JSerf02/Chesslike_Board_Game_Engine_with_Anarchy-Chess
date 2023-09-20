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
            /* Adds all of the correct pieces to the board */
            void setup();
        
        public:
            /* Create a ChessBoard with all of the correct pieces */
            ChessBoard(std::vector<Piece::Player> players = {}) : GameBoard(players) {
                setup();
            }
            
            /*
             * Returns true if the input is in the square chess board (all coordinates are in the interval [1, 8])
            */
            bool onBoard(int x, int y) override;
    };

}
#endif