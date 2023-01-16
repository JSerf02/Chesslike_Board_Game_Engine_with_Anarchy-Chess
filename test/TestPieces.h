#ifndef TESTPIECES_H
#define TESTPIECES_H

// A piece that has moves with 1 priority
class Priority1Piece : public Piece
{
    public:
        using Piece::Piece;
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {{{std::make_pair(1, 1)}, 1}};
        } 
};

// A piece that has moves with 2 priority
class Priority2Piece : public Piece
{
    public:
        using Piece::Piece;
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {{{std::make_pair(2, 2)}, 2}};
        } 
};

// A piece that has moves with 3 priority
class Priority3Piece : public Piece
{
    public:
        using Piece::Piece;
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {{{std::make_pair(3, 3)}, 3}};
        } 
};

// A piece that has moves with 4 priority
class Priority4Piece : public Piece
{
    public:
        using Piece::Piece;
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {{{std::make_pair(4, 4)}, 4}};
        } 
};

// A piece that has moves with 5 priority
class Priority5Piece : public Piece
{
    public:
        using Piece::Piece;
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {{{std::make_pair(5, 5)}, 5}};
        } 
};

// A piece that has 5 moves with priority 5 and 1 move of priority 1
class FiveFivesOneOnePiece : public Piece
{
    public:
        using Piece::Piece;
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {{{std::make_pair(1, 1)}, 5}, {{std::make_pair(1, 1)}, 5}, {{std::make_pair(1, 1)}, 5}, {{std::make_pair(1, 1)}, 5}, {{std::make_pair(1, 1)}, 5}, {{std::make_pair(1, 1)}, 1}};
        } 
};

// A piece that has 5 moves to different positions =
class FiveDifferentPositionsPiece : public Piece
{
    public:
        using Piece::Piece;
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {{{std::make_pair(11, 11)}, 5}, {{std::make_pair(12, 12)}, 5}, {{std::make_pair(13, 13)}, 5}, {{std::make_pair(14, 14)}, 5}, {{std::make_pair(15, 15)}, 5}};
        } 
};

class OneTwoTwoThreeAttack : public Piece
{
    public:
        using Piece::Piece;
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {};
        }
        std::vector<Move> generateAttackingMoves(GameState& gameState) override
        {
            return {{{std::make_pair(1, 1)}}, {{std::make_pair(3, 3)}}, {{std::make_pair(2, 2), std::make_pair(3, 3)}}};
        } 
};

class OneTwoAttack : public Piece
{
    public:
        using Piece::Piece;
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {};
        }
        std::vector<Move> generateAttackingMoves(GameState& gameState) override
        {
            return {{{std::make_pair(1, 2)}}};
        } 
};

class OneThreeAttack : public Piece
{
    public:
        using Piece::Piece;
        std::vector<Move> generateMoves(GameState& gameState) override
        {
            return {};
        }
        std::vector<Move> generateAttackingMoves(GameState& gameState) override
        {
            return {{{std::make_pair(1, 3)}}};
        } 
};

#endif