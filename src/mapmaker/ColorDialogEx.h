// (c) 1997 Roger Onslow

#pragma once

class CColorDialogEx : public CColorDialog
{
// Construction
public:
	DECLARE_DYNCREATE(CColorDialogEx);
	CColorDialogEx(COLORREF cr = RGB(0,0,0), DWORD Flags = 0, CWnd* pParentWnd = NULL );
	
// Overrides
	//{{AFX_VIRTUAL(CColorDialogEx)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	enum { k_CustomColorCount = 16 };
	static COLORREF m_CustomColors[k_CustomColorCount];
	static COLORREF m_LastCustomColors[k_CustomColorCount];
	static bool		m_InitCustomColors;

	static void InitCustomColors();
	static void SaveCustomColors();

// Message map
protected:
	//{{AFX_MSG(CColorDialogEx)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
