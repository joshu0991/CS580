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

class GPuzzleManager : public Utils::GManager
{
public:
    GPuzzleManager(void) : Utils::GManager()
    {
	m_pos   = 0;
	m_board = NULL;
	m_tmax      = 3.0;
	
    }
    
    virtual ~GPuzzleManager(void)
    {
	if(m_board)
	    m_puzzle.DeleteBoard(m_board);
    }

    virtual void HandleEventOnDisplay(void)
    {
	Utils::GManager::HandleEventOnDisplay();
	
	char nr[100];
	
	Utils::GDrawSetParam(Utils::GDRAW_MINX, 0);
	Utils::GDrawSetParam(Utils::GDRAW_MINY, 0);
	Utils::GDrawSetParam(Utils::GDRAW_MAXX, m_puzzle.GetNrCols());
	Utils::GDrawSetParam(Utils::GDRAW_MAXY, m_puzzle.GetNrRows());
	
	Utils::GDrawColor(0, 0, 0);
	Utils::GDrawGrid2D(0, 0, m_puzzle.GetNrCols(), m_puzzle.GetNrRows(),
			   m_puzzle.GetNrCols(), m_puzzle.GetNrRows());
	
	for(int r = m_puzzle.GetNrRows() - 1; r >= 0; --r)
	    for(int c = m_puzzle.GetNrCols() - 1; c >= 0; --c)
	    {
		const int val = m_board->GetValue(m_puzzle.IdFromRowCol(r, c));
	
		if(val > 0)
		    Utils::GDrawColor(0.3, 0.3, 0.7);
		else
		    Utils::GDrawColor(0.1, 0.1, 0.1);
		
		Utils::GDrawAABox2D(c + 0.1, m_puzzle.GetNrRows() - r - 1 + 0.1, c + 0.9, m_puzzle.GetNrRows() - r - 0.1);
		
		sprintf(nr, "%d", val);
		Utils::GDrawColor(1, 1, 0);
		Utils::GDrawString2D(nr, c + 0.5, m_puzzle.GetNrRows() - r - 0.5);
	    }
    }

    virtual void Help(void)
    {
	printf("PRESS KEY\n");	
	printf("r : randomize puzzle board\n");
	printf("0 : solve puzzle using the zero heuristic\n");
	printf("1 : solve puzzle using heuristic1\n");
	printf("2 : solve puzzle using heuristic2\n");
	printf("3 : solve puzzle using heuristic3\n");
	printf("+ : go over the solution one move at a time\n");
	printf("p : read puzzle board from file puzzle.txt\n");
	printf("m : read moves from file moves.txt\n");       
	printf("ARROW_LEFT  : make left move\n");
	printf("ARROW_RIGHT : make right move\n");
	printf("ARROW_UP    : make up move\n");
	printf("ARROW_DOWN  : make down move\n");
	printf("h : print these messages\n");
    }
    
    
    virtual bool HandleEventOnSpecialKeyPress(const int key)
    {
	if(key == GLUT_KEY_LEFT)
	    printf("move left: ok = %d\n", m_puzzle.MakeMoveLeft(m_board));
	else if(key == GLUT_KEY_RIGHT)
	    printf("move right: ok = %d\n", m_puzzle.MakeMoveRight(m_board));
	else if(key == GLUT_KEY_UP)
	    printf("move up: ok = %d\n", m_puzzle.MakeMoveUp(m_board));
	else if(key == GLUT_KEY_DOWN)
	    printf("move down: ok = %d\n", m_puzzle.MakeMoveDown(m_board));
	
	return true;	
    }
    
    virtual bool HandleEventOnNormalKeyPress(const int key)
    {
	if(key == '+')
	{
	    if(m_pos >= 0 && m_pos < (int) m_moves.size())
	    {
		m_puzzle.MakeMove(m_board, m_moves[m_pos]);
		++m_pos;
	    }
	}
	else if(key == 'r')
	{
	    RunRandomizer();
	}	
	else if(key == '0')
	{
	    printf("..running puzzle solver with h0 heuristic\n");
	    RunSolver(&m_heuristic0);
	}
	else if(key == '1')
	{
	    printf("..running puzzle solver with h1 heuristic\n");
	    RunSolver(&m_heuristic1);
	}
	else if(key == '2')
	{
	    printf("..running puzzle solver with h2 heuristic\n");
	    RunSolver(&m_heuristic2);
	}
	else if(key == '3')
	{
	    printf("..running puzzle solver with h3 heuristic\n");
	    RunSolver(&m_heuristic3);
	}
	else if(key == 'p')
	{
	    Puzzle::Board *b = m_puzzle.ReadBoard("puzzle.txt");
	    if(b != NULL)
		m_puzzle.DeleteBoard(m_board);
	    m_board = b;
	}
	else if(key == 'm')
	{
	    m_puzzle.ReadMoves("moves.txt", &m_moves);
	    m_pos = 0;
	}
	else if(key == 'h')
	    Help();
	
	
	
	
	GManager::HandleEventOnNormalKeyPress(key);
	
	return true;
    }

    void RunRandomizer(void)
    {
	printf("..randomizing board\n");
	for(int i = m_puzzle.GetNrSquares(); i >= 0; --i)
	    m_board->SetValue(i, i);
	m_board->SetIdEmpty(0);
	m_puzzle.Randomize(m_board, m_nrRandMoves);
    }
    
    void RunSolver(PuzzleHeuristic *h)
    {
	    m_moves.clear();
	    m_pos = 0;
	    Utils::Timer::Clock clk;
	    Utils::Timer::Start(&clk);
	    const bool solved = m_solver.Solve(&m_puzzle,  h, m_board, m_tmax, &m_moves);
	    printf("..solved = %d nrMoves = %d time = %f\n", solved, (int) m_moves.size(), Utils::Timer::Elapsed(&clk));

    }
    
    
    Puzzle           m_puzzle;
    Puzzle::Board   *m_board;
    PuzzleSolver     m_solver;
    PuzzleHeuristic  m_heuristic0;
    PuzzleHeuristic1 m_heuristic1;
    PuzzleHeuristic2 m_heuristic2;
    PuzzleHeuristic3 m_heuristic3;

    std::vector<Puzzle::Move> m_moves;
    int                       m_pos;
    double                    m_tmax;
    int                       m_nrRandMoves;
    
};


int GRunPuzzle(int argc, char **argv)
{ 
    GPuzzleManager gManager;

    printf("usage: GRunPuzzle nrRows nrCols nrRandMoves tmax\n");
    

    const int nrRows      = argc > 1 ? atoi(argv[1]) : 3;
    const int nrCols      = argc > 2 ? atoi(argv[2]) : 3;


    gManager.m_nrRandMoves = argc > 3 ? atoi(argv[3]) : 20;
    gManager.m_tmax        = argc > 4 ? atof(argv[4]) : 3.0;
    
    printf("..using nrRows        = %d\n", nrRows);
    printf("..using nrCols        = %d\n", nrCols);
    printf("..using nrRandomMoves = %d\n", gManager.m_nrRandMoves);
    printf("..using tmax          = %f\n", gManager.m_tmax);
    
    
    gManager.m_puzzle.SetNrRows(nrRows);
    gManager.m_puzzle.SetNrCols(nrCols);
    gManager.m_board = gManager.m_puzzle.NewBoard();
    
    gManager.RunRandomizer();
    
    gManager.Help();
    
    gManager.MainLoop("GPuzzle", 400, 600);
    
    return 0;
}

