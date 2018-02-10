#include "Utils/GManager.hpp"
#include "Puzzle/Puzzle.hpp"
#include "Puzzle/PuzzleSolver.hpp"
#include "Puzzle/PuzzleHeuristic1.hpp"
#include "Puzzle/PuzzleHeuristic2.hpp"
#include "Puzzle/PuzzleHeuristic3.hpp"
#include "Utils/Timer.hpp"
#include "Utils/PseudoRandom.hpp"
#include <cstdio>
#include <cstring>

int RunRandomPuzzle(int argc, char **argv);
int RunPuzzleSolver(int argc, char **argv);
int GRunPuzzle(int argc, char **argv);

int main(int argc, char **argv)
{ 
    Utils::RandomSeed();
    
    if(argc < 2)
    {
	printf("usage: ./bin/Runner <program-name> <program-args>\n");
	printf("  <program-name>: RunRandomPuzzle, RunPuzzleSolver, or GRunPuzzle\n");
	return 0;
    }
    

    const char *prog = argv[1];
    
    if(strcmp(prog, "RunRandomPuzzle") == 0)
	return RunRandomPuzzle(argc - 1, &argv[1]);
    else if(strcmp(prog, "RunPuzzleSolver") == 0)
	return RunPuzzleSolver(argc - 1, &argv[1]);
    else if(strcmp(prog, "GRunPuzzle") == 0)
	return GRunPuzzle(argc - 1, &argv[1]);
    else
	printf("..unknown program name: expecting RunRandomPuzzle, RunPuzzleSolver, or GRunPuzzle\n");
    
    
    return 0;
}

