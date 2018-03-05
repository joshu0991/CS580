#ifndef PUZZLE_HEURISTIC1_HPP_
#define PUZZLE_HEURISTIC1_HPP_

#include "Puzzle/PuzzleHeuristic.hpp"

class PuzzleHeuristic1 : public PuzzleHeuristic
{
public:
    PuzzleHeuristic1(void) : PuzzleHeuristic()
    {
    }
    
    virtual ~PuzzleHeuristic1(void)
    {
    }

    //you should implement this function
    virtual double Evaluate(Puzzle *puzzle, Puzzle::Board *board);
    
};

#endif
