#ifndef PIECE_H
#define PIECE_H

class GameBoard;
class Piece
{
    public:
        Piece();
        void Test(int x);
        void virtual GenerateMoves(const GameBoard& gameBoard) { }
};

#endif