#include "Puzzle/PuzzleSolver.hpp"
#include "Puzzle/PuzzleHeuristic1.hpp"
#include "Puzzle/PuzzleHeuristic2.hpp"
#include "Puzzle/PuzzleHeuristic3.hpp"
#include "Utils/PseudoRandom.hpp"
#include "Utils/Timer.hpp"
#include <cstdio>
#include <cstring>

int RunPuzzleSolver(int argc, char **argv)
{ 
    printf("usage: RunPuzzleSolver fnameBoard heuristic tmax fnameMoves\n");
    
    const char  *fnameIn = argc > 1 ? argv[1] : "puzzle.txt";
    const char  *hname   = argc > 2 ? argv[2] : NULL;
    const double tmax    = argc > 3 ? atof(argv[3]) : 3.0;
    const char  *fnameOut= argc > 4 ? argv[4] : "moves.txt";
    

    printf("..using fnameBoard = %s\n", fnameIn);
    printf("..using heuristic  = %s\n", hname);
    printf("..using tmax       = %f\n", tmax);
    printf("..using fnameMoves = %s\n", fnameOut);
     
    Puzzle puzzle;
    Puzzle::Board *b = puzzle.ReadBoard(fnameIn);
    PuzzleSolver solver;
    PuzzleHeuristic *h = NULL;
    std::vector<Puzzle::Move> moves;
    
    
    if(hname == NULL)
	h = new PuzzleHeuristic();
    else if(strcmp(hname, "H1") == 0)
	h = new PuzzleHeuristic1();
    else if(strcmp(hname, "H2") == 0)
	h = new PuzzleHeuristic2();
    else if(strcmp(hname, "H3") == 0)
	h = new PuzzleHeuristic3();
    
   
    Utils::Timer::Clock clk;

    Utils::Timer::Start(&clk);
    const bool solved = solver.Solve(&puzzle,  h, b, tmax, &moves);
    printf("..solved = %d nrMoves = %d time = %f\n", solved, (int) moves.size(), Utils::Timer::Elapsed(&clk));

    printf("..checking solution\n");
    for(int i = 0; i < (int) moves.size(); ++i)
	puzzle.MakeMove(b, moves[i]);
    if(puzzle.IsSolved(b) == false)
    {
	printf("..did not reach goal configuration, see\n");
	puzzle.PrintBoard(NULL, b);
    }
    else 
	printf("..ok\n");
    
    
    puzzle.PrintMoves(fnameOut, &moves);
    
    
    puzzle.DeleteBoard(b);
    delete h;
    
    
    return 0;
}

