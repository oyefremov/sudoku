// SudokuWnd.cpp : implementation file
//

#include "stdafx.h"
#include "sudoku.h"
#include "SudokuWnd.h"


// CSudokuWnd

IMPLEMENT_DYNAMIC(CSudokuWnd, CWnd)

CSudokuWnd::CSudokuWnd()
:currX(0)
,currY(0)
{

}

CSudokuWnd::~CSudokuWnd()
{
}


BEGIN_MESSAGE_MAP(CSudokuWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CSudokuWnd message handlers
void CSudokuWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	dc.SetBkMode(TRANSPARENT);

	dc.FillSolidRect(0, 0, fieldSize, fieldSize, RGB(255, 255, 255));

	if (currX >= 0 && currY >= 0)
	{
		dc.FillSolidRect(
			currX * cellSize, 
			currY * cellSize,
			cellSize, 
			cellSize, 
			RGB(255, 200, 200));
	}


	CPen pen_light(PS_DOT, 1, RGB(200, 200, 200)), pen_black(PS_SOLID, 2, RGB(0, 0, 0)), *pCurPen;

	pCurPen = dc.SelectObject(&pen_light);
	for (int i=0; i<10; i++)
	{
		dc.MoveTo(0, i * cellSize);
		dc.LineTo(fieldSize, i * cellSize);
		dc.MoveTo(i * cellSize, 0);
		dc.LineTo(i * cellSize, fieldSize);
	}

	dc.SelectObject(&pen_black);
	for (int i=0; i<10; i+=3)
	{
		dc.MoveTo(0, i * cellSize);
		dc.LineTo(fieldSize, i * cellSize);
		dc.MoveTo(i * cellSize, 0);
		dc.LineTo(i * cellSize, fieldSize);
	}
	dc.SelectObject(pCurPen );
	
	CFont bigFont, smallFont, *pOldFont;
	bigFont.CreatePointFont(280, _T("Arial"));
	smallFont.CreatePointFont(80, _T("Arial"));

	pOldFont = dc.SelectObject(&bigFont);
	for (int x = 0; x<9; x++)
		for (int y = 0; y<9; y++)
		{
			const CCell * pCell = game.GetCell(x, y);
			const int textXOffset = 14;
			const int textYOffset = 4;			
			if (pCell->GetType() == LOADED_FROM_FILE)
				dc.SetTextColor(RGB(0, 0, 0));
			else if (pCell->GetType() == USER_DEFINED)
				dc.SetTextColor(RGB(255, 0, 128));
			else if (pCell->GetType() == CALCULATED_SAVED)
				dc.SetTextColor(RGB(0, 128, 0));
			else if (pCell->GetType() == CALCULATED)
				dc.SetTextColor(RGB(64, 255, 64));
			else continue;

			wchar_t val = L'0' + pCell->GetValue();
			dc.TextOutW(
				textXOffset + x * cellSize, 
				textYOffset + y * cellSize,
				&val, 
				1);
		}

	dc.SelectObject(&smallFont);
	for (int x = 0; x<9; x++)
		for (int y = 0; y<9; y++)
		{
			const CCell * pCell = game.GetCell(x, y);
			const int textXOffset = 14;
			const int textYOffset = 4;
			if (pCell->HasValue()) continue;

			dc.SetTextColor(RGB(0, 128, 128));
			int val = pCell->GetNumberOfPossiblevalues();
			COLORREF colors[] = 
			{
				0,  // 0
				RGB(0, 255, 0),   // 1
				RGB(150, 150, 150),  // 2 
				RGB(150, 150, 150),  // 3
				RGB(150, 150, 150),  // 4 
				RGB(150, 150, 150),  // 5 
				RGB(150, 150, 150),  // 6 
				RGB(150, 150, 150),  // 7 
				RGB(150, 150, 150),  // 8 
				RGB(150, 150, 150)   // 9 
			};

			dc.SetTextColor(colors[val]);

			const int xpos = x * cellSize + 3;
			const int ypos = y * cellSize + 3;
			for (int i=0; i<3; i++)
			for (int j=0; j<3; j++)
			{
				const int v = i + j * 3 + 1;
				const wchar_t ch = v + '0';
				if (pCell->CanSetValue(v))
				{
					if (pCell->IsTheOnlyPlaceForValue(v))
						dc.SetTextColor(colors[1]);
					else
						dc.SetTextColor(colors[val]);

					dc.TextOutW(
						xpos + i * 9, 
						ypos + j * 12, 
						&ch, 1);
				}
			}
		}
	dc.SelectObject(pOldFont);
}

void CSudokuWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	int x = point.x / cellSize;
	int y = point.y / cellSize;
	if (x >= 0 && x <= 9 && y >= 0 && y <= 9) 
	{
		currX = x;
		currY = y;
		Invalidate(FALSE);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CSudokuWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CCell* pCell = game.GetCell(currX, currY);

	switch (nChar)
	{
	//case 38:
	//	if (--currY < 0) currY = 8;
	//	break;
	//case 40:
	//	if (++currY > 8) currY = 0;
	//	break;
	//case 37:
	//	if (--currX < 0) currX = 8;
	//	break;
	//case 39:
	//	if (++currX > 8) currX = 0;
	//	break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		{
			int val = nChar - '0';
			if (pCell->CanSetValue(val))
			{
				game.Push();
				pCell->SetUserValue(val);
				Check();
			}
			break;
		}
	case ' ':
	case '0':
		if (pCell->HasValue())
		{
			game.Push();
			pCell->ResetCurrentValue();
			Check();
		}
		break;
	case 'S':
		game.Save(_T("sudoku.txt"));
		break;
	case 'L':
		game.Load(_T("sudoku.txt"));
		break;
	case 'P':
		game.Push();
		break;
	case 'O':
		if (game.CanPop())
		{
			game.Pop();
			Check();
		}
		break;
	case 'A':
		game.Push();
		if (!game.Auto())
			MessageBox(L"No auto turn is available");
		break;
	case 'Q':
		Check();
		break;
	}
	Invalidate();

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSudokuWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	int x = point.x / cellSize;
	int y = point.y / cellSize;
	if (x >= 0 && x <= 9 && y >= 0 && y <= 9) 
	{
		currX = x;
		currY = y;

		CCell* pCell = game.GetCell(x, y);
		if (pCell->AutoFill())
			Invalidate(FALSE);
	}

	CWnd::OnRButtonDown(nFlags, point);
}

void CSudokuWnd::Check()
{
	SetWindowText(_T("Thinking ..."));
	UpdateWindow();
	int time = GetTickCount();
	LPCTSTR msg = game.TryToSolveMsg();
	CString s;
	s.Format(_T("%s (in %i ms)"), msg, GetTickCount() - time);
	SetWindowText(s);
}

int CSudokuWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect clientRect, wndRect; 
	GetClientRect(clientRect);
	GetWindowRect(wndRect);
	int dx = clientRect.Width() - fieldSize;
	int dy = clientRect.Height() - fieldSize;
	wndRect.right -= dx;
	wndRect.bottom -= dy - 30;
	MoveWindow(wndRect);

	return 0;
}
