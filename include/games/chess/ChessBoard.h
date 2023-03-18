#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <unordered_map>

#include "GameState.h"

namespace chess {
    using namespace logic;
    using Player = Piece::Player;

    class ChessBoard : public GameBoard
    {
        private:
            /*
             * Adds all of the chess pieces to the board as they are in normal chess
            */
            void setup() override;
        public:
            /*
             * Returns true if the input is in the square chess board (1 <= x, y <= 8)
            */
            bool onBoard(int x, int y) override;
    };

}
#endif