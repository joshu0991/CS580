#include "Puzzle/PuzzleHeuristic1.hpp"

#include <cstdint>

double PuzzleHeuristic1::Evaluate(Puzzle *puzzle, Puzzle::Board *board)
{
    // misplaced tiles heuristic
    double counter = 0;
    for(std::int32_t i = puzzle->GetNrSquares() - 1; i >= 0; --i)
    {
        if(board->GetValue(i) != i)
            ++counter;
    }

    return counter;
}

