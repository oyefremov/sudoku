#pragma once

#include "Group.h"
#include "Cell.h"

class CSudokuGame
{
	CCell cells[9][9];
	CGroup rows[9], cols[9], squares[9];
public:
	CSudokuGame(void);
	~CSudokuGame(void);

	const CCell* GetCell(int i, int j) const {return &cells[i][j];}
	CCell* GetCell(int i, int j) {return &cells[i][j];}

	bool Auto();
	// return number of possible solutions:\
	// 0 - no solutions
	// 1 - single solution
	// 2 - multiple solutions
	int TryToSolve();
	LPCTSTR TryToSolveMsg();

	void Save(LPCTSTR filename) const;
	bool Load(LPCTSTR filename);

	void Push();
	void Pop();
	bool CanPop() const;
};

