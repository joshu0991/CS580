#ifndef PUZZLE_HEURISTIC2_HPP_
#define PUZZLE_HEURISTIC2_HPP_

#include "Puzzle/PuzzleHeuristic.hpp"

class PuzzleHeuristic2 : public PuzzleHeuristic
{
public:
    PuzzleHeuristic2(void) : PuzzleHeuristic()
    {
    }
    
    virtual ~PuzzleHeuristic2(void)
    {
    }
    
    //you should implement this function
    virtual double Evaluate(Puzzle *puzzle, Puzzle::Board *board);
    
};

#endif
