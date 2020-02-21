#include "StdAfx.h"
#include "SudokuGame.h"

CSudokuGame::CSudokuGame(void)
{
	for (int x = 0; x<9; x++)
		for (int y = 0; y<9; y++)
		{
			cells[x][y].SetGroups(
				cols + x, 
				rows + y, 
				// should be x/3 + y/3*3
				squares + (x / 3 + y / 3 * 3));
		}
}

CSudokuGame::~CSudokuGame(void)
{
}

void CSudokuGame::Save(LPCTSTR filename) const
{
	CStdioFile file;
	if (file.Open(filename, CFile::modeCreate | CFile::modeWrite))
	{		
		for (int j=0; j<9; j++)
		{
			TCHAR line[11];
			for (int i=0; i<9; i++)
			{
				char val = '0' + cells[i][j].GetValue();
				if (val == '0') val = '.';
				line[i] = val;
			}
			line[9] = '\n';
			line[10] = 0;
			file.WriteString(line);
		}
	}	
}
bool CSudokuGame::Load(LPCTSTR filename)
{
	CStdioFile file;
	if (file.Open(filename, CFile::modeRead))
	{		
		for (int j=0; j<9; j++)
			for (int i=0; i<9; i++)
				cells[i][j].ResetCurrentValue();

		CString s;

		for (int j=0; j<9; j++)
		{
			if (!file.ReadString(s))
				break;
			for (int i=0; i<min(9, s.GetLength()) ; i++)
			{
				TCHAR val = s[i];
				if (val >= '1' && val <= '9')
					cells[i][j].SetLoadedValue(val - '0');
			}
		}

		return true;
	}	
	return false;
}

CArray<CSudokuGame> g_stack;

void CSudokuGame::Push() 
{
	g_stack.Add(*this);
	for (int j=0; j<9; j++)
		for (int i=0; i<9; i++)
		{
			if (cells[i][j].GetType() == CALCULATED)
				cells[i][j].SetType(CALCULATED_SAVED);
		}
}

void CSudokuGame::Pop()
{
	size_t last = g_stack.GetSize() - 1;
	*this = g_stack[last];
	g_stack.RemoveAt(last);
}

bool CSudokuGame::CanPop() const
{
	return !g_stack.IsEmpty();
}

bool CSudokuGame::Auto()
{
	bool bRes = false;
	
BEGIN:
	
	for (int j=0; j<9; j++)
		for (int i=0; i<9; i++)
			if (cells[i][j].AutoFill())
			{
				bRes = true;
				goto BEGIN;
			}

	return bRes;
}

int CSudokuGame::TryToSolve()
{
	int num_solutions = 0;

	Push();
	Auto();
	// find cell with minimum available numbers
	int min_num = 10;
	CCell *pBestCell = 0;

	for (int j=0; j<9; j++)
		for (int i=0; i<9; i++)
		{
			CCell *p = &cells[i][j];
			if (p->HasValue()) continue;

			int num = p->GetNumberOfPossiblevalues();
			if (num < min_num)
			{
				min_num = num;
				pBestCell = p;
				if (min_num == 0) 
					goto END;
			}
		}

	if (!pBestCell)
	{
		// solved 
		num_solutions = 1;
	}
	else 
	{
		CArray<int> arr;
		pBestCell->GetAllPossibleValues(arr);
		
		for (int i=0; i<arr.GetSize(); i++)
		{
			pBestCell->SetValue(arr[i]);
			num_solutions += TryToSolve();
			if (num_solutions > 1)
			{
				// multiple solutions
				num_solutions = 2;
				break;
			}
		}
	}

END:
	Pop();
	return num_solutions;
}
LPCTSTR CSudokuGame::TryToSolveMsg()
{
	const LPCTSTR Msg[] = {
			_T("No solutions"), 
			_T("Single solution"),
			_T("Multiple solutions")
	};
	return Msg[TryToSolve()];
}