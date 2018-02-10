#include "Puzzle/Puzzle.hpp"
#include "Utils/PseudoRandom.hpp"
#include <cstdio>

void Puzzle::SetNrRows(const int nrRows)
{
    m_nrRows = nrRows;
}

void Puzzle::SetNrCols(const int nrCols)
{
    m_nrCols = nrCols;
}

int Puzzle::GetNrRows(void) const
{
    return m_nrRows;
}

int Puzzle::GetNrCols(void) const
{
    return m_nrCols;
}

int Puzzle::GetNrSquares(void) const
{
    return GetNrRows() * GetNrCols();
}

Puzzle::Board* Puzzle::NewBoard(void) const
{
    Board *b = new Board();
    b->m_squares = new int[GetNrSquares()];
    return b;    
}

void Puzzle::DeleteBoard(Board *b) const
{
    if(b)
	delete b;
}

Puzzle::Board* Puzzle::CopyBoard(const Board *b) const
{
    Board *bnew = NewBoard();
    for(int i = GetNrSquares() - 1; i >= 0; --i)
	bnew->SetValue(i, b->GetValue(i));
    bnew->SetIdEmpty(b->GetIdEmpty());
    return bnew;
}

int Puzzle::IdFromRowCol(const int r, const int c) const
{
    return r * GetNrCols() + c;
}

int Puzzle::RowFromId(const int id) const
{
    return id / GetNrCols();
}

int Puzzle::ColFromId(const int id) const
{
    return id % GetNrCols();
}


void Puzzle::MakeMove(Puzzle::Board *b, int from, int to) const
{
    const int fromVal = b->GetValue(from);
    b->SetValue(from, b->GetValue(to));
    b->SetValue(to, fromVal);
    b->SetIdEmpty(to);
}


bool Puzzle::MakeMoveRight(Puzzle::Board *b) const
{
    const int id = b->GetIdEmpty();
    const int c  = ColFromId(id);
    const int r  = RowFromId(id);
    
    if(c < GetNrCols() - 1)
    {
	MakeMove(b, id, IdFromRowCol(r, c + 1));
	return true;
    }
    return false;
}

bool Puzzle::MakeMoveLeft(Puzzle::Board *b) const
{
    const int id = b->GetIdEmpty();
    const int c  = ColFromId(id);
    const int r  = RowFromId(id);
    
    if(c > 0)
    {
	MakeMove(b, id, IdFromRowCol(r, c - 1));
	return true;
    }
    return false;
}


bool Puzzle::MakeMoveUp(Puzzle::Board *b) const
{
    const int id = b->GetIdEmpty();
    const int c  = ColFromId(id);
    const int r  = RowFromId(id);
    
    if(r > 0)
    {
	MakeMove(b, id, IdFromRowCol(r - 1, c));
	return true;
    }
    return false;
}

bool Puzzle::MakeMoveDown(Puzzle::Board *b) const
{
    const int id = b->GetIdEmpty();
    const int c  = ColFromId(id);
    const int r  = RowFromId(id);
    
    if(r < GetNrRows() - 1)
    {
	MakeMove(b, id, IdFromRowCol(r + 1, c));
	return true;
    }
    return false;
}

bool Puzzle::MakeMove(Puzzle::Board *b, const Puzzle::Move move) const
{
    switch(move)
    {
    case MOVE_LEFT: return MakeMoveLeft(b);
    case MOVE_RIGHT: return MakeMoveRight(b);
    case MOVE_UP: return MakeMoveUp(b);
    case MOVE_DOWN: return MakeMoveDown(b);
    };
}


bool Puzzle::IsSolved(const Puzzle::Board *b) const
{
    for(int i = GetNrSquares() - 1; i >= 0; --i)
	if(b->GetValue(i) != i)
	    return false;
    return true;
}

void Puzzle::Randomize(Board *b, const int nrMoves) const
{
    int count = 0;
    while(count < nrMoves)
    {
	const double coin = Utils::RandomUniformReal();
	if(coin < 0.25)
	    count += MakeMoveLeft(b);
	else if(coin < 0.5)
	    count += MakeMoveRight(b);
	else if(coin < 0.75)
	    count += MakeMoveDown(b);
	else 
	    count += MakeMoveUp(b);
    }
}


Puzzle::Board* Puzzle::ReadBoard(const char fname[])
{
    FILE *in = fopen(fname, "r");
    int   val;
    
    if(!in)
    {
	printf("..could not open <%s> for reading\n", fname);
	return NULL;
    }
    
    if(fscanf(in, "%d %d", &m_nrRows, &m_nrCols) != 2)
    {
	printf("..error: expecting nrRows nrCols in file <%s>\n", fname);
	fclose(in);
	return NULL;
    }
    
    Board *board = NewBoard();
    
    for(int i = 0; i < GetNrSquares(); ++i)
    {
	if(fscanf(in, "%d", &val) != 1)
	{
	    printf("..error: expecting value for square %d\n", i);
	    fclose(in);
	    return board;
	}
	board->SetValue(i, val);
	if(val == 0)
	    board->SetIdEmpty(i);
    }
    
    fclose(in);
 
    return board;
    
}

void Puzzle::ReadMoves(const char fname[], std::vector<Move> *moves) const
{
    FILE *in = fopen(fname, "r");
    int   val;
    
    if(!in)
    {
	printf("..could not open <%s> for reading\n", fname);
	return;
    }
    
    moves->clear();
    while(fscanf(in, "%d", &val) == 1)
    {
	if(val >= 0 && val <= 3)
	    moves->push_back((Move) val);
	else 
	    printf("..unspecified move value %d...ignored\n", val);
    }
    
    printf("..read %d moves\n", (int) moves->size());
    fclose(in);
}

void Puzzle::PrintBoard(const char fname[], const Board *board) const
{
    FILE *out = fname == NULL ? stdout : fopen(fname, "w");
    if(!out)
    {
	printf("..could not open <%s> for writing\n", fname);
	return;
    }
    fprintf(out, "%d %d\n", GetNrRows(), GetNrCols());
    for(int r = 0; r < GetNrRows(); ++r)
    {
	for(int c = 0; c < GetNrCols(); ++c)
	    fprintf(out, "%3d ", board->GetValue(IdFromRowCol(r, c)));
	fprintf(out, "\n");
    }
    if(fname != NULL)
	fclose(out);
    
}

    
void Puzzle::PrintMoves(const char fname[], const std::vector<Move> *moves) const
{
    FILE *out = fopen(fname, "w");
    if(!out)
    {
	printf("..could not open <%s> for writing\n", fname);
	return;
    }
    for(int i = 0; i < (int) moves->size(); ++i)
	fprintf(out, "%d\n", (*moves)[i]);
    fclose(out);
}

    
    
    
