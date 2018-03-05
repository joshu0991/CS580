#include "Puzzle/PuzzleSolver.hpp"

#include <algorithm>
#include <chrono>

namespace
{
    bool compareBoards(Puzzle* p_puzzle, Puzzle::Board* p_lhs, Puzzle::Board* p_rhs)
    {
        for(int i = p_puzzle->GetNrSquares() - 1; i >= 0; --i)
        {   
            if(p_lhs->GetValue(i) != p_rhs->GetValue(i))
                return false;
        }   
        return true;
    }

    void generateSuccessors(Puzzle* p_puzzle, Puzzle::Board* p_board, 
        std::vector< std::shared_ptr< Node > >& p_returnPermutations)
    {
        const int id = p_board->GetIdEmpty();
        const int c  = p_puzzle->ColFromId(id);
        const int r  = p_puzzle->RowFromId(id);

        Puzzle::Board* boardCopy = NULL;
        // can a move be made up?
        if (r > 0)
        {
            // a move can be made up. Copy the board
            boardCopy = p_puzzle->CopyBoard(p_board);
            p_puzzle->MakeMoveUp(boardCopy);

            // create a new node for returning.
            std::shared_ptr< Node > node(new Node(boardCopy, Puzzle::MOVE_UP));

            // insert the node into the return vector
            p_returnPermutations.push_back(node);
            boardCopy = NULL;
        }
        
        // can a move be made down?
        if (r < p_puzzle->GetNrRows() - 1)
        {
            // a move can be made up. Copy the board
            boardCopy = p_puzzle->CopyBoard(p_board);
            p_puzzle->MakeMoveDown(boardCopy);

            // create a new node for returning.
            std::shared_ptr< Node > node(new Node(boardCopy, Puzzle::MOVE_DOWN));

            // insert the node into the return vector
            p_returnPermutations.push_back(node);
            boardCopy = NULL;
        }

        // can a move be made right?
        if (c < p_puzzle->GetNrCols() - 1)
        {
            // a move can be made up. Copy the board
            boardCopy = p_puzzle->CopyBoard(p_board);
            p_puzzle->MakeMoveRight(boardCopy);

            // create a new node for returning.
            std::shared_ptr< Node > node(new Node(boardCopy, Puzzle::MOVE_RIGHT));

            // insert the node into the return vector
            p_returnPermutations.push_back(node);
            boardCopy = NULL;
        }

        // can a move be made left?
        if (c > 0)
        {
            // a move can be made up. Copy the board
            boardCopy = p_puzzle->CopyBoard(p_board);
            p_puzzle->MakeMoveLeft(boardCopy);

            // create a new node for returning.
            std::shared_ptr< Node > node(new Node(boardCopy, Puzzle::MOVE_LEFT));

            // insert the node into the return vector
            p_returnPermutations.push_back(node);
            boardCopy = NULL;
        }
    }
}

bool PuzzleSolver::discard(Puzzle* p_puzzle, std::shared_ptr< Node > p_toCheck) const
{
    // check to see if there is a node in identical to this one
    // in the open list which has a better score then this one
    for (auto element : m_openSet)
    {
        // if the boards are the same and the element in the set
        // has a lower score than the generated successor return true
        if (compareBoards(p_puzzle, element->m_board, p_toCheck->m_board) &&
            element->m_total < p_toCheck->m_total)
        {
            return true;
        }
    }

    for (auto element : m_closedSet)
    {
        if (compareBoards(p_puzzle, element->m_board, p_toCheck->m_board) &&
            element->m_total < p_toCheck->m_total)
        {
            return true;
        }
    }
    return false;
}

bool PuzzleSolver::Solve(Puzzle *puzzle, 
			 PuzzleHeuristic *h, 
			 Puzzle::Board *board, 
			 const double tmax, 
			 std::vector<Puzzle::Move> *moves)
{
    // both open and closed sets are already initialized in constructor

    // vector for next states
    std::vector< std::shared_ptr< Node > > returnVector;

    // create a node for the starting board with no
	// parent and f value set to 0
    std::shared_ptr< Node > start(new Node(board, Puzzle::MOVE_UNINIT));

	// put the starting node on the open list
    m_priorityQueue.push(start);
    m_openSet.insert(start);

    bool foundGoal = false;
    std::shared_ptr< Node > builder = NULL;
    std::chrono::time_point<std::chrono::system_clock> startTime = 
        std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> updateTime = 
        std::chrono::system_clock::now();
    std::chrono::duration< double > diff;
    while (!foundGoal && !m_priorityQueue.empty())
    {
        // update the timer
        updateTime = std::chrono::system_clock::now();
        diff = updateTime - startTime; 
        if (diff.count() >= tmax)
        {
            return false;
        }

        // obtain and extract the smallest element
        std::shared_ptr< Node > q = m_priorityQueue.top();
        m_openSet.erase(q);
        m_priorityQueue.pop();

        // generate successors
        generateSuccessors(puzzle, q->m_board, returnVector);

        // for each successor
        for(auto successor : returnVector)
        {
            // set the parent first incase of a solution
            successor->m_parent = q;

            // is this the goal if so we are done.
            if (puzzle->IsSolved(successor->m_board))
            {
                foundGoal = true;
                builder = successor;
                break;
            }

            // otherwise update the "tree"

            // the cost from the previous to this is 1
            successor->m_goal = q->m_goal + 1;
            successor->m_heuristic = h->Evaluate(puzzle, successor->m_board);
            successor->m_total = successor->m_goal + successor->m_heuristic;

            // check to ensure we want this successor in the open set
            // if the node has the same orientation as something in the 
            // open set and has a total cost less than this node, we discard
            // this node
            if (!discard(puzzle, successor))
            {
                // then add the node to the open list
                m_openSet.insert(successor);
                m_priorityQueue.push(successor);
            }
        }
        // done with q insert into the closed set
        m_closedSet.insert(q);
        returnVector.clear();
    }

    // while the parent is not null
    while (builder->m_parent)
    {
        moves->push_back(builder->m_direction);

        // find the parent should be in the closed set
        // this does poitner comparison
        builder = *(m_closedSet.find(builder->m_parent)); 
    }
    // reverse the order of the moves
    std::reverse(moves->begin(), moves->end());

    return true;
}

