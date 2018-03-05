#include "Puzzle/Puzzle.hpp"
#include "Utils/PseudoRandom.hpp"
#include <cstdio>


int RunRandomPuzzle(int argc, char **argv)
{ 
    printf("usage: RunRandomPuzzle nrRows nrCols nrRandMoves fnameOut\n");
    

    const int   nrRows  = argc > 1 ? atoi(argv[1]) : 3;
    const int   nrCols  = argc > 2 ? atoi(argv[2]) : 3;
    const int   nrMoves = argc > 3 ? atoi(argv[3]) : 20;
    const char *fname   = argc > 4 ? argv[4] : "puzzle.txt";
    
    printf("..using nrRows        = %d\n", nrRows);
    printf("..using nrCols        = %d\n", nrCols);
    printf("..using nrRandomMoves = %d\n", nrMoves);
    printf("..using fname         = %s\n", fname);
     
    Puzzle puzzle;
    puzzle.SetNrRows(nrRows);
    puzzle.SetNrCols(nrCols);
    
    Puzzle::Board *b = puzzle.NewBoard();
    
    for(int i = puzzle.GetNrSquares(); i >= 0; --i)
	b->SetValue(i, i);
    b->SetIdEmpty(0);
    puzzle.Randomize(b, nrMoves);

    puzzle.PrintBoard(fname, b);
    
    puzzle.DeleteBoard(b);
    
    
    return 0;
}

