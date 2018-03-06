#include "Programs/TTT.hpp"

#include <cstdio>
#include <algorithm>
#include <cmath>

#include <iostream>
double TTT::EvalState(const State s) const
{
    Value winner = checkRowsForWin(s);
    if (winner != UNINIT)
    {
        return winner == FIRST ? 1 : -1;
    }
    
    winner = checkColumnForWin(s);
    if (winner != UNINIT)
    {
        return winner == FIRST ? 1 : -1;
    }

    winner = checkDiagonalsForWin(s);
    if (winner != UNINIT)
    {
        return winner == FIRST ? 1 : -1;
    }

    return 0;
}	


void TTT::BestMove(State   s, 
		   int     depth, 
		   int     remMoves, 
		   int     lastMove,
		   Value   player,
		   double *bestScore,
		   int    *bestMove) const
{
    double alpha = -1 * MAX_VALUE;
    double beta = MAX_VALUE;
    *bestScore = alphaBetaMiniMax(s,
                                  depth, 
                                  remMoves, 
                                  lastMove,
                                  alpha,
                                  beta,
                                  player, 
                                  bestMove);
}


double TTT::NegaMax(State   s, 
		    int     depth, 
		    double  alpha, 
		    double  beta, 
		    int     remMoves, 
		    int     lastMove,
		    int     color) const
{
    //add your implementation
    return 0.0;
    
}

bool TTT::CheckWin(const State s, const int r, const int c) const
{
    const Value val  = GetValueState(s, r, c);
    int   cons = 0, end;
    
    //check row
    cons = 0; end = std::min(m_nrCols, c + m_nrToWin);
    for(int i = std::max(0, c - m_nrToWin); i < end; ++i)
	if(GetValueState(s, r, i) != val)
	    cons = 0;
	else if((++cons) >= m_nrToWin)
	    return true;
    
    //check column
    cons = 0; end = std::min(m_nrRows, r + m_nrToWin);
    for(int i = std::max(0, r - m_nrToWin); i < end; ++i)
	if(GetValueState(s, i, c) != val)
	    cons = 0;
	else if((++cons) >= m_nrToWin)
	    return true;
    
    //check diagonal
    cons = 0; end = 1 + std::min(m_nrToWin - 1, std::min(m_nrRows - r - 1, m_nrCols - c - 1));
    for(int i = std::max(-m_nrToWin + 1, std::max(-r, -c)); i < end; ++i)
	if(GetValueState(s, r + i, c + i) != val)
	    cons = 0;
	else if((++cons) >= m_nrToWin)
	    return true;
    
    //check anti-diagonal
    cons = 0; end = 1 + std::min(m_nrToWin - 1, std::min(r, m_nrCols - c - 1));
    for(int i = std::max(-m_nrToWin + 1, std::max(r - m_nrRows + 1, -c)); i < end; ++i)
    {
	if(GetValueState(s, r - i, c + i) != val)
	    cons = 0;
	else if((++cons) >= m_nrToWin)
	    return true;
    }
    
    
    return false;
    
    
}

void TTT::generateMoves(State p_state, 
                        std::vector< std::pair< std::int32_t,State > >& p_moves, 
                        Value p_player) const 
{
    // generate all of the possible moves for this state and this player and 
    // return the values in the p_moves vector

    // for each space on the board
    for (std::uint32_t iter = 0; iter < m_nrRows * m_nrCols; ++iter)
    {
        // check to see if the space is occupied
        // a space is not occipied if the value
        // at i is zero 
        if (!p_state[iter])
        {
            // we have found a new possible 
            // configuration create and push back
            State temp = CopyState(p_state);
            temp[iter] = p_player;
            p_moves.push_back(std::make_pair(iter, temp));
        }
    } 
}

double TTT::alphaBetaMiniMax(State p_state, 
                             int p_depth, 
                             int p_remMoves,
                             int p_lastMove,
                             double p_alpha,
                             double p_beta,
                             Value p_player,
                             int* p_bestMove) const
{
    // terminal test / base case
    if (!p_depth || terminalTest(p_state, p_lastMove))
    {
        std::cout << "---- eval state: " << 
            static_cast< int >(p_state[0]) <<
            static_cast< int >(p_state[1]) <<
            static_cast< int >(p_state[2]) <<
            static_cast< int >(p_state[3]) <<
            static_cast< int >(p_state[4]) <<
            static_cast< int >(p_state[5]) <<
            static_cast< int >(p_state[6]) <<
            static_cast< int >(p_state[7]) <<
            static_cast< int >(p_state[8]) <<
            " at depth " << p_depth << 
            std::endl;
        // we have a winning board 
        // return it's value
        return EvalState(p_state);
    }

    if (p_player == FIRST)
    {
        return maxValue(p_state,
                        p_depth,
                        p_remMoves,
                        p_lastMove,
                        p_alpha,
                        p_beta,
                        p_player,
                        p_bestMove); 
    }

    if (p_player == SECOND)
    {
        return minValue(p_state,
                        p_depth,
                        p_remMoves,
                        p_lastMove,
                        p_alpha,
                        p_beta,
                        p_player,
                        p_bestMove); 
    }
}

bool TTT::terminalTest(State p_state, int p_lastMove) const
{
    if (p_lastMove < 0)
    {
        return false;
    }

    return CheckWin(p_state, RowFromId(p_lastMove), ColFromId(p_lastMove)) ||
           boardFull(p_state);
}

bool TTT::boardFull(const State p_state) const
{
    for (std::uint32_t iter = 0; iter < m_nrRows * m_nrCols; ++iter)
    {
        if (!p_state[iter])
        {
            return false;
        }
    }
    return true;
}

TTT::Value TTT::checkRowsForWin(State p_state) const
{
    // check the rows to see if player 1
    // or player 2 was the victor
    Value winner = UNINIT;
    for (std::uint32_t row = 0; row < m_nrRows; ++row)
    {
        Value startOfRun = GetValueState(p_state, row, 0);
        Value next = UNINIT;
        std::int32_t numberInRowCounter = 1;
        
        for (std::uint32_t column = 1; column < m_nrCols; ++column)
        {
            next = GetValueState(p_state, row, column);
            if (next != startOfRun || 
                next == EMPTY || startOfRun == EMPTY)
            {
                winner = UNINIT;
                if (m_nrToWin != m_nrRows)
                {
                    // the row streak of the first
                    // ends at this column rest the counter
                    numberInRowCounter = 1;

                    // we have to keep checking to see
                    // if there are a run of nrToWin since
                    // we can't guarantee m_nrRows == m_nrToWin
                    startOfRun = next;
                    continue;
                }
                else
                {
                    // if the number to win is the same as number in 
                    // a row then there can't be a continuous run of 
                    // winning length hence we are done
                    break;
                }
            }
            ++numberInRowCounter;
            if (numberInRowCounter == m_nrToWin)
            {
                winner = GetValueState(p_state, row, column);
                break;
            }
        }

        if (winner != UNINIT)
        {
            return winner;
        } 
    }
    return UNINIT;
}

TTT::Value TTT::checkColumnForWin(State p_state) const
{
    // check the columns to see if player 1
    // or player 2 was the victor
    Value winner = UNINIT;
    for (std::uint32_t column = 0; column < m_nrCols; ++column)
    {
        Value next = UNINIT;
        Value startOfRun = GetValueState(p_state, 0, column);
        std::int32_t numberInColCounter = 1;

        for (std::uint32_t row = 1; row < m_nrRows; ++row)
        {
            next = GetValueState(p_state, row, column);
            if (next != startOfRun || 
                !next)
            {
                if (m_nrToWin != m_nrCols ||
                    next == EMPTY || startOfRun == EMPTY)
                {
                    // the row streak of the first
                    // ends at this column rest the counter
                    numberInColCounter = 1;

                    // we have to keep checking to see
                    // if there are a run of nrToWin since
                    // we can't guarantee m_nrRows == m_nrToWin
                    startOfRun = next;
                    continue;
                }
                else
                {
                    // if the number to win is the same as number in 
                    // a row then there can't be a continuous run of 
                    // winning length hence we are done
                    break;
                }
            }
            ++numberInColCounter;
            if (numberInColCounter == m_nrToWin)
            {
                winner = GetValueState(p_state, row, column);
                break;
            }

        }

        if (winner != UNINIT)
        {
            return winner;
        } 
    }
    return UNINIT;
}

TTT::Value TTT::checkDiagonalsForWin(State p_state) const
{
    // check the diagonals for a run of length
    // m_nrToWin diagonal 1 from top left to bottom
    // right is at index 0, 4, 8, etc diagonal 2 is
    // from top right to bottom left 2, 4, 6, etc

    // check the main diagonal the next diagonal entry is at number
    // of rows +1 
    Value startOfRun = GetValueState(p_state, 0, 0);
    Value next = UNINIT;
    Value winner = UNINIT;
    std::int32_t numberInDiagCounter = 1;

    if (startOfRun)
    {
        for (std::uint32_t iter = (m_nrCols + 1); 
             iter < (m_nrRows * m_nrCols); 
             iter += (m_nrCols + 1))
        {
            next = static_cast< Value >(p_state[iter]);
            if (next != startOfRun ||
                !next)
            {
                startOfRun = next;
                numberInDiagCounter = 1;
                continue;
            }
            
            ++numberInDiagCounter;
            if (numberInDiagCounter == m_nrToWin)
            {
                winner = static_cast< Value >(p_state[iter - (m_nrCols + 1)]);
                break;
            }
        }

        if (winner != UNINIT)
        {
            return winner;
        }
    }

    startOfRun = GetValueState(p_state, 0, m_nrCols - 1);
    next = UNINIT;
    winner = UNINIT;
    numberInDiagCounter = 1;

    if (startOfRun)
    {
        for (std::uint32_t iter = (2 * m_nrCols - 2); 
             iter < (m_nrRows * m_nrCols) - 1; 
             iter += (m_nrCols - 1))
        {
            next = static_cast< Value >(p_state[iter]);
            if (next != startOfRun ||
                !next)
            {
                startOfRun = next;
                numberInDiagCounter = 1;
                continue;
            }
   
            ++numberInDiagCounter;
            if (numberInDiagCounter == m_nrToWin)
            {
                winner = next;
                break;
            }
        }

        if (winner != UNINIT)
        {
            return winner;
        }
    }
    return UNINIT;
}

double TTT::maxValue(State p_state, 
                     int p_depth, 
                     int p_remMoves,
                     int p_lastMove,
                     double p_alpha,
                     double p_beta,
                     Value p_player,
                     int* p_bestMove) const
{
    // vector of the possible moves, the first in the pair
    // is the last move made for the board
    std::vector< std::pair< std::int32_t, State > > moves;
    generateMoves(p_state, moves, p_player);
    double v = -1 * MAX_VALUE;
    double tempV = -1 * MAX_VALUE;
    
    // first is value of move second is move
    std::pair< double, int> lastBest;

    for (auto& successor : moves)
    {
        tempV = alphaBetaMiniMax(successor.second,
                                         p_depth - 1,
                                         p_remMoves - 1,
                                         successor.first,
                                         p_alpha,
                                         p_beta,
                                         p_player == FIRST ? SECOND : FIRST,
                                         p_bestMove);
        std::cout << "Max returned " << tempV << " for " <<
            static_cast< int >(p_state[0]) <<
            static_cast< int >(p_state[1]) <<
            static_cast< int >(p_state[2]) <<
            static_cast< int >(p_state[3]) <<
            static_cast< int >(p_state[4]) <<
            static_cast< int >(p_state[5]) <<
            static_cast< int >(p_state[6]) <<
            static_cast< int >(p_state[7]) <<
            static_cast< int >(p_state[8]) << " at depth "
            << p_depth <<
            std::endl;
        if (tempV > v)
        {
            v = tempV;
            lastBest = std::make_pair(v, successor.first);
        }

        if (v >= p_beta)
        {
            *p_bestMove = lastBest.second;
      
            return v;
        }
        p_alpha = std::max(p_alpha, v);
    }
    *p_bestMove = lastBest.second;
    return v;
}

double TTT::minValue(State p_state, 
                     int p_depth, 
                     int p_remMoves,
                     int p_lastMove,
                     double p_alpha,
                     double p_beta,
                     Value p_player,
                     int* p_bestMove) const
{
    // vector of the possible moves, the first in the pair
    // is the last move made for the board
    std::vector< std::pair< std::int32_t, State > > moves;
    generateMoves(p_state, moves, p_player);
    double v = MAX_VALUE;
    double tempV = MAX_VALUE;

    // first is the move value second is the move
    std::pair< double, int > lastBest;

    for (auto& successor : moves)
    {
        tempV = alphaBetaMiniMax(successor.second,
                                         p_depth - 1,
                                         p_remMoves - 1,
                                         successor.first,
                                         p_alpha,
                                         p_beta,
                                         p_player == FIRST ? SECOND : FIRST,
                                         p_bestMove);
        std::cout << "Min returned " << tempV << " for " <<
            static_cast< int >(p_state[0]) <<
            static_cast< int >(p_state[1]) <<
            static_cast< int >(p_state[2]) <<
            static_cast< int >(p_state[3]) <<
            static_cast< int >(p_state[4]) <<
            static_cast< int >(p_state[5]) <<
            static_cast< int >(p_state[6]) <<
            static_cast< int >(p_state[7]) <<
            static_cast< int >(p_state[8]) << " at depth "
            << p_depth <<
            std::endl;
        if (tempV < v)
        {
            v = tempV;
            lastBest = std::make_pair(v, successor.first);
        } 
        
        if (v <= p_alpha)
        {
            *p_bestMove = lastBest.second;

            return v;
        }
       
        p_beta = std::min(p_beta, v);
    }
    *p_bestMove = lastBest.second;
    return v;
}

