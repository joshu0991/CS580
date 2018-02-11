#include "Puzzle/PuzzleHeuristic3.hpp"

#include <cmath>
#include <cstdint>

double PuzzleHeuristic3::Evaluate(Puzzle *puzzle, Puzzle::Board *board)
{
    // Tiles out of row and column heuristic implementation
    
    double counter = 0;
    std::int32_t puzzleRow = 0;
    std::int32_t puzzleColumn = 0;
    std::int32_t id = 0;
    for(std::uint32_t i = 0; i < puzzle->GetNrSquares(); ++i)
    {
        // update the puzzle row and column
        puzzleRow    = puzzle->RowFromId(i);    
        puzzleColumn = puzzle->ColFromId(i);    

        // the element at this i value
        id = board->GetValue(i);

        // check to see if the tile is out of row
        // if it is increment the counter
        if (puzzleRow != puzzle->RowFromId(id))
        {
            ++counter;
        }

        // check to see if the tile is out of column
        // if it is increment the counter
        if (puzzleColumn != puzzle->ColFromId(id))
        {
            ++counter;
        }
    }

    return counter;
}

