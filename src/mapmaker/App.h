
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMapMaker:
//

class CMainFrame;

class CMapMaker : public CWinApp
{
public:
	CMapMaker();

// Attributes
public:
	CString			m_CloseCombatPath;	// path to close combat root folder, ie: "c:\games\cc2\"
	CElementArray	m_Elements;
	CMenu			m_ElementMenu;		// context menu built from elements

	CMainFrame*		GetMainFrame();

// Operations
public:
	void InvalidateAllViews();
	void PrintStatus(const CString& Text, int Pane = 0, bool Update = false);

// Overrides
	//{{AFX_VIRTUAL(CMapMaker)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ReadProgramSettings();
	void WriteProgramSettings();

	BOOL ReadElements();
	BOOL CreateElementMenu();

// Message Map
	//{{AFX_MSG(CMapMaker)
	afx_msg void OnAppAbout();
	afx_msg void OnTerrainColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

extern CMapMaker	g_App;
