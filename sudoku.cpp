// sudoku.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "sudoku.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSudokuApp

BEGIN_MESSAGE_MAP(CSudokuApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSudokuApp construction

CSudokuApp::CSudokuApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSudokuApp object

CSudokuApp theApp;


// CSudokuApp initialization

BOOL CSudokuApp::InitInstance()
{
	CWinApp::InitInstance();
		
	wnd.CreateEx(WS_EX_APPWINDOW | WS_EX_CLIENTEDGE, AfxRegisterWndClass(0), _T("Sudoku"), 
		WS_CAPTION | WS_VISIBLE | WS_POPUPWINDOW, 
		CRect(100, 100, 0, 0), 0, 0);
	m_pMainWnd = &wnd;
	return TRUE;
}
