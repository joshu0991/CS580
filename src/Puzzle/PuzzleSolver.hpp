#ifndef PUZZLE_SOLVER_HPP_
#define PUZZLE_SOLVER_HPP_

#include "Puzzle/Puzzle.hpp"
#include "Puzzle/PuzzleHeuristic.hpp"

#include <cstdint>
#include <functional>
#include <memory>
#include <queue>
#include <set>
#include <vector>

struct Node 
{
    Node(Puzzle::Board* p_board, Puzzle::Move p_move) :
        m_parent(NULL),
        m_board(p_board),
        m_total(0),
        m_goal(0),
        m_heuristic(0),
        m_direction(p_move)
    {
    }

    std::shared_ptr< Node > m_parent;
    Puzzle::Board* m_board;
    std::uint64_t m_total;
    std::uint64_t m_goal;
    double m_heuristic;
    Puzzle::Move m_direction;
};

struct DerefComparator : public 
    std::binary_function< std::unique_ptr< Node >, std::shared_ptr< Node >, bool>
{
    bool operator()(const std::shared_ptr< Node > p_lhs, std::shared_ptr< Node > p_rhs)
    {
        return p_lhs->m_total < p_rhs->m_total;
    }	
};

class PuzzleSolver
{
public:
    PuzzleSolver(void) :
        m_priorityQueue(),
        m_openSet(),
        m_closedSet()
    {
    }
    
    virtual ~PuzzleSolver(void)
    {
    }

    //you should implement this function   
    //board is the start configuration    
    //return true if solution found; false otherwise
    //add the solution moves to the moves vector
    virtual bool Solve(Puzzle *puzzle, 
		       PuzzleHeuristic *h, 
		       Puzzle::Board *board, 
		       const double tmax, 
		       std::vector<Puzzle::Move> *moves);

    //! \return true iff the given node should be discarded
    bool discard(Puzzle* p_puzzle, std::shared_ptr< Node > p_toCheck) const; 

public:
	//! Priority queue for the open set
    std::priority_queue< std::shared_ptr< Node >, 
                         std::vector< std::shared_ptr< Node > >,
                         DerefComparator > 
    m_priorityQueue;

    //! for tracking nodes in the open set
    std::set< std::shared_ptr < Node > > m_openSet;
    
    //! the closed set
    std::set< std::shared_ptr< Node > > m_closedSet;	 
};

#endif

