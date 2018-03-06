#ifndef AI__TTT_HPP_
#define AI__TTT_HPP_

#include <cstdint>
#include <cstring>
#include <utility>
#include <vector>

//define the maximum value for the evaluation function,
//which should return a value in the range [-MAX_VALUE, MAX_VALUE]
#define MAX_VALUE 100000000000

class TTT
{
public:
//initial values for nrRows, nrCols, and nrToWin
//these values are changed to the values specified by the user when the program is run
    TTT(void) : m_nrRows(3),
		m_nrCols(3),
		m_nrToWin(3)
    {
    }
    
    
//Variables representing the number of rows, number of columns, and number of consecutive elements needed for a win
    int m_nrRows;
    int m_nrCols;
    int m_nrToWin;
    
    
//EMPTY value denotes an empty square
//FIRST value denotes a square occupied by the first player
//SECOND value denotes a square occupied by the second player
    enum Value
	{
	    EMPTY  = 0,
	    FIRST  = 1,
	    SECOND = 2,
        UNINIT = 3
	};
    
//State of the tic-tac-toe is presented as a one-dimensional array (similar to puzzle board)
//There are also functions (see GetValueState, SetValueState) to get and set the state values
//at different rows and columns
    typedef unsigned char *State;
 
//This is a utility function which converts row, col into id
    int IdFromRowCol(const int row, const int col) const
    {
	return row * m_nrCols + col;
    }
   
//extract row from id 
    int RowFromId(const int id) const
    {
	return id / m_nrCols;
    }
 
//extract column from id   
    int ColFromId(const int id) const
    {
	return id % m_nrCols;
    }
    

//This is a utility function which returns the value in position (row, col)
    Value GetValueState(const State s, const int row, const int col) const
    {
	return (Value) s[row * m_nrCols + col];
    }
    
//This is a utility function which sets the square in position (row, col) to val
//For example, SetValue(s, 1, 2, FIRST) sets the square in position (1, 2) to the value FIRST 
// (marks the move by the first player)
    void SetValueState(State s, const int row, const int col, const Value val) const
    {
	s[row * m_nrCols + col] = val;
    }
    
//This is a utility function which checks if the player that made the move in position (r, c) actually won the game.
//Note that (r, c) corresponds to the last move made. The function returns true iff this last move made is a winning move.
    bool CheckWin(const State s, const int r, const int c) const;
    
    
//This is the function that you need to write 
//(write it in the src/Programs/TTT.cpp file; here it's just the definition of the function).
    double EvalState(const State s) const;
    
    
//This function uses minimax with alpha-beta prunning and the evaluation function to select the best move
//s is the board configuration
//depth is the maximum search depth
//remMoves is the number of remaining moves (the game ends in a tie if remMoves is zero)
//lastMove indicates the last move made by a player
//bestScore is an output variable, which will contain the highest score for the player
//bestMove is an output variable, which will contain the best move for the player	  
    void BestMove(State   s, 
		  int     depth, 
		  int     remMoves, 
		  int     lastMove,
		  Value   player,
		  double *bestScore,
		  int    *bestMove) const;
    
    
//This function uses minimax with alpha-beta prunning and the evaluation function. 
//s is the board configuration
//depth is the maximum search depth
//remMoves is the number of remaining moves (the game ends in a tie if remMoves is zero)
//lastMove indicates the last move made by a player
//color indicates the turn (1 for FIRST and -1 for SECOND player)
    double NegaMax(State   s, 
		   int     depth, 
		   double  alpha, 
		   double  beta, 
		   int     remMoves, 
		   int     lastMove,
		   int     color) const;
    
    
    
//This is a utility function, which allocates memory for a new state		
    State NewState(void) const
    {
	return new unsigned char[m_nrRows * m_nrCols];
    }
    
    
//This is a utility function, which deletes the memory allocated to a state
    void DeleteState(State s) const
    {
	if(s)
	    delete [] s;
    }
    
//This is a utility function which sets every square to 0
    void ClearState(State s) const
    {
	memset(s, 0, m_nrRows * m_nrCols * sizeof(unsigned char));
    }
    
//This is a utility function which assigns copies the values of the state src to the state dest
    void CopyState(const State src, State dest) const
    {
	memcpy(dest, src, m_nrRows * m_nrCols * sizeof(unsigned char));
    }
    
//This is a utility function which creates a new state as an exact copy of the state s
    State CopyState(const State s) const
    {
	State dest = NewState();
	CopyState(s, dest);
	return dest;
    }

    /*!
     * Main recursive dispatching function for alpha beta minimax
     *
     * \param[in] p_state the state to evaluate
     * \param[in] p_depth the current depth of the recursion tree
     * \param[in] p_remMoves the remaining moves
     * \param[in] p_lastMove the last move a player made
     * \param[in] p_alpha the best move for max at a level in the tree
     * \param[in] p_beta the best move for min at a level in the tree
     * \param[in] p_player the current player to considers move
     * \param[out] p_bestMove the best move for a player
     *
     * \note alpha and beta do not get passed up to the parent only
     *       down to the next tree level v is then used for updating
     *       alpha and beta at a given level
     *
     * \return the score of the best move
     *
     */
    double alphaBetaMiniMax(State p_state, 
                            int p_depth, 
                            int p_remMoves,
                            int p_lastMove,
                            double p_alpha,
                            double p_beta,
                            Value p_player,
                            int* p_bestMove) const;

    /*!
     *
     * Recusive function for the max players move choice
     *
     * \param[in] p_state the state to evaluate
     * \param[in] p_depth the current depth of the recursion tree
     * \param[in] p_remMoves the remaining moves
     * \param[in] p_lastMove the last move a player made
     * \param[in] p_alpha the best move for max at a level in the tree
     * \param[in] p_beta the best move for min at a level in the tree
     * \param[in] p_player the current player to considers move
     * \param[out] p_bestMove the best move for a player
     *
     * \note alpha and beta do not get passed up to the parent only
     *       down to the next tree level v is then used for updating
     *       alpha and beta at a given level
     *
     * \return the score of the best move for max
     *
     */
    double maxValue(State p_state, 
                    int p_depth, 
                    int p_remMoves,
                    int p_lastMove,
                    double p_alpha,
                    double p_beta,
                    Value p_player,
                    int* p_bestMove) const;

    /*!
     *
     * Recursive call for the min players move choice
     *
     * \param[in] p_state the state to evaluate
     * \param[in] p_depth the current depth of the recursion tree
     * \param[in] p_remMoves the remaining moves
     * \param[in] p_lastMove the last move a player made
     * \param[in] p_alpha the best move for max at a level in the tree
     * \param[in] p_beta the best move for min at a level in the tree
     * \param[in] p_player the current player to considers move
     * \param[out] p_bestMove the best move for a player
     *
     * \note alpha and beta do not get passed up to the parent only
     *       down to the next tree level v is then used for updating
     *       alpha and beta at a given level
     *
     * \return the score of the best move
     *
     */
    double minValue(State p_state, 
                    int p_depth, 
                    int p_remMoves,
                    int p_lastMove,
                    double p_alpha,
                    double p_beta,
                    Value p_player,
                    int* p_bestMove) const;

    /*!
     *
     * Determine if a state is a leaf node
     *
     * \param[in] p_state the state to check
     * \param[in] p_lastMove the last move made
     */
    bool terminalTest(State p_state, int p_lastMove) const;

    /*!
     *
     * Generate all of the permutations of a board for a player
     *
     * \param[in] p_state the start state for the permutations
     * \param[out] p_moves A vector containing pairs which represent
     *             the move states possible and their "lastMove"
     * \param[in] p_player the player to generate moves for
     *
     */
    void generateMoves(State p_state, 
                       std::vector< std::pair <std::int32_t, State > >& p_moves, 
                       Value p_player) const;

    /*!
     *
     * Check if a given state is "full"
     * \note this could be done simply by checking the
     *       remaining moves variable
     *
     * \param[in] p_state the state to check
     *
     * \return true iff there are no remaining moves
     *
     */
    bool boardFull(const State p_state) const;

    /*!
     *
     * Check the rows in a state for a victory
     *
     * \param[in] p_state the state to evaluate
     *
     * \return the value of the player if there is a victor
     *         or an uninitialized state if no rows cause a win
     *
     */
    Value checkRowsForWin(State p_state) const;

    /*!
     *
     * Check the columns in a state for a victory
     *
     * \param[in] p_state the state to evaluate
     *
     * \return the value of the player if there is a victor
     *         or an uninitialized state if no columns cause a win
     *
     */
    Value checkColumnForWin(State p_state) const;
    
    /*!
     *
     * Check the diagonals in a state for a victory
     *
     * \param[in] p_state the state to evaluate
     *
     * \return the value of the player if there is a victor
     *         or an uninitialized state if no diagonals cause a win
     *
     */
    Value checkDiagonalsForWin(State p_state) const;
};


#endif

