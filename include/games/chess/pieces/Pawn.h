#ifndef PAWN_H
#define PAWN_H

#include "ChessPiece.h"

namespace chess {
    using namespace logic;

    class Pawn : public ChessPiece
    {
        /*
         * TODO: Define Pawn class
        */
        private:
            bool alreadyMoved{};
            int boostTurn{};

            /*
            * Returns whether the pawn can boost
            * - The pawn can boost if it has not moved previously
            */
            bool canBoost() { return false; } // Pawn.cpp doesn't exist yet so leaving TODO implementation here
        
        public:
            using ChessPiece::ChessPiece;

            /*
            * Returns whether the pawn boosted on the previoius turn
            */
            bool justBoosted() { return false; } // Pawn.cpp doesn't exist yet so leaving TODO implementation here


    };

}
#endif