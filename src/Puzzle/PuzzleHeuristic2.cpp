#include "Puzzle/PuzzleHeuristic2.hpp"

#include <cmath>
#include <cstdint>

double PuzzleHeuristic2::Evaluate(Puzzle *puzzle, Puzzle::Board *board)
{
    // calculate manhattan distance

    // get the row size 
    std::uint32_t rowSize = puzzle->GetNrCols();
    
    double counter = 0;
    std::int32_t puzzleRow = 0;
    std::int32_t puzzleColumn = 0;
    std::int32_t id = 0;
    for(std::uint32_t i = 0; i < puzzle->GetNrSquares(); ++i)
    {
        // update the puzzle row and column
        puzzleRow    = puzzle->RowFromId(i);    
        puzzleColumn = puzzle->ColFromId(i);    

        // for every squarre on the board calculate the
        // manhatten distance to it's correct place
        // start by getting the current id at x,y
        id = board->GetValue(i); // puzzle->IdFromRowCol(puzzleRow, puzzleColumn);
        
        // calculate the x and y components different
        // and take the absolute value add it to th
        // running counter for the manhattan distance
        counter += (std::abs(puzzleRow - puzzle->RowFromId(id)) + 
                    std::abs(puzzleColumn - puzzle->ColFromId(id)));
    
    }

    return counter;
}

