#ifndef PUZZLE_HEURISTIC3_HPP_
#define PUZZLE_HEURISTIC3_HPP_

#include "Puzzle/PuzzleHeuristic.hpp"

class PuzzleHeuristic3 : public PuzzleHeuristic
{
public:
    PuzzleHeuristic3(void) : PuzzleHeuristic()
    {
    }
    
    virtual ~PuzzleHeuristic3(void)
    {
    }

        //you should implement this function
    virtual double Evaluate(Puzzle *puzzle, Puzzle::Board *board);
    
};

#endif
