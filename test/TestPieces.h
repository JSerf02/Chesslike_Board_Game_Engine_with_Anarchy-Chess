#ifndef TESTPIECES_H
#define TESTPIECES_H

namespace testing {

    // A piece that has moves with 1 priority
    class Priority1Piece : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(1, 1)}, 1}};
            } 
    };

    // A piece that has moves with 2 priority
    class Priority2Piece : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(2, 2)}, 2}};
            } 
    };

    // A piece that has moves with 3 priority
    class Priority3Piece : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(3, 3)}, 3}};
            } 
    };

    // A piece that has moves with 4 priority
    class Priority4Piece : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(4, 4)}, 4}};
            } 
    };

    // A piece that has moves with 5 priority
    class Priority5Piece : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(5, 5)}, 5}};
            } 
    };

    // A piece that has 5 moves with priority 5 and 1 move of priority 1
    class FiveFivesOneOnePiece : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(1, 1)}, 5}, {{std::make_pair(1, 1)}, 5}, {{std::make_pair(1, 1)}, 5}, {{std::make_pair(1, 1)}, 5}, {{std::make_pair(1, 1)}, 5}, {{std::make_pair(1, 1)}, 1}};
            } 
    };

    // A piece that has 5 moves to different positions =
    class FiveDifferentPositionsPiece : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(11, 11)}, 5}, {{std::make_pair(12, 12)}, 5}, {{std::make_pair(13, 13)}, 5}, {{std::make_pair(14, 14)}, 5}, {{std::make_pair(15, 15)}, 5}};
            } 
    };

    class OneTwoTwoThreeAttack : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {};
            }
            std::vector<logic::Move> generateAttackingMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(1, 1)}}, {{std::make_pair(3, 3)}}, {{std::make_pair(2, 2), std::make_pair(3, 3)}}};
            } 
    };

    class OneTwoAttack : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {};
            }
            std::vector<logic::Move> generateAttackingMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(1, 2)}}};
            } 
    };

    class OneThreeAttack : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {};
            }
            std::vector<logic::Move> generateAttackingMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(1, 3)}}};
            } 
    };

    class BigPriority : public logic::Piece
    {
        private:
            using logic::Piece::Piece;
            std::vector<logic::Move> generateMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(1, 1)}}};
            }
            std::vector<logic::Move> generateAttackingMoves(logic::GameState& gameState) override
            {
                return {{{std::make_pair(1, 1)}}};
            } 
        public:
            int getMinPriority(logic::GameState& gameState) override
            {
                return 5001; // It's over 5000!
            }
    };
}
#endif