#ifndef PUZZLE_HPP_
#define PUZZLE_HPP_

#include <cstdlib>
#include <vector>

class Puzzle
{
public:
    enum Move
	{
	    MOVE_LEFT   = 0,
	    MOVE_RIGHT  = 1,
	    MOVE_UP     = 2,
	    MOVE_DOWN   = 3,
        MOVE_UNINIT = 4
	};

    struct Board
    {
	friend class Puzzle;
	
	Board(void) : m_squares(NULL),
		      m_empty(-1)
	{
	}
	
	virtual ~Board(void)
	{
	    if(m_squares)
		delete[] m_squares;
	}

	int GetValue(const int id) const
	{
	    return m_squares[id];
	}
	
	int GetIdEmpty(void) const
	{
	    return m_empty;
	}
	
	void SetValue(const int id, const int val)
	{
	    m_squares[id] = val;
	}
	
	void SetIdEmpty(const int id)
	{
	    m_empty = id;
	}

    protected:	
	int *m_squares;
	int  m_empty;
    };

    Puzzle(void) : m_nrRows(3),
		   m_nrCols(3)
    {
    }
    
    
    virtual ~Puzzle(void)
    {
    }

    virtual void SetNrRows(const int nrRows);
    virtual void SetNrCols(const int nrCols);
    
    virtual int GetNrRows(void) const;
    virtual int GetNrCols(void) const;
    virtual int GetNrSquares(void) const;
    virtual Board* NewBoard(void) const;
    virtual void DeleteBoard(Board *b) const;
    virtual Board* CopyBoard(const Board *b) const;
        
    virtual int IdFromRowCol(const int r, const int c) const;
    virtual int RowFromId(const int id) const;
    virtual int ColFromId(const int id) const;	
    virtual bool MakeMoveLeft(Board *b) const;
    virtual bool MakeMoveRight(Board *b) const;
    virtual bool MakeMoveUp(Board *b) const;
    virtual bool MakeMoveDown(Board *b) const;
    virtual bool MakeMove(Board *b, const Move move) const;
    virtual bool IsSolved(const Board *b) const;
    virtual void Randomize(Board *b, const int nrMoves) const;

    virtual Board* ReadBoard(const char fname[]);
    virtual void ReadMoves(const char fname[], std::vector<Move> *moves) const;
    virtual void PrintBoard(const char fname[], const Board *board) const;
    virtual void PrintMoves(const char fname[], const std::vector<Move> *moves) const;
    
protected:
    virtual void MakeMove(Board *b, int from, int to) const;
    
    int m_nrRows;
    int m_nrCols;
    
};
    
#endif
    
