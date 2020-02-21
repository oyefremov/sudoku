// sudoku.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "SudokuWnd.h"		// main symbols

// CSudokuApp:
// See sudoku.cpp for the implementation of this class
//

class CSudokuApp : public CWinApp
{
	CSudokuWnd wnd;
public:
	CSudokuApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSudokuApp theApp;