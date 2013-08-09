// TileColorDlg.h : header file
//

#pragma once

#include "ColourPicker.h"

/////////////////////////////////////////////////////////////////////////////
// CTileColorDlg dialog

class CTileColorDlg : public CDialog
{
// Construction
public:
	CTileColorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTileColorDlg)
	enum { IDD = IDD_TILE_COLOR };
	CComboBox	m_TileCtrl;
	CColourPicker m_ColorCtrl;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTileColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTileColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTileChange();
	//}}AFX_MSG
    afx_msg LONG OnColorChange(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};
