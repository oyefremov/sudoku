#pragma once

#include "SudokuGame.h"

// CSudokuWnd

class CSudokuWnd : public CWnd
{
	void Check();
	CSudokuGame game;
	int currX;
	int currY;

	DECLARE_DYNAMIC(CSudokuWnd)

public:
	CSudokuWnd();
	virtual ~CSudokuWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


const int cellSize = 48;
const int fieldSize = 9 * cellSize;
