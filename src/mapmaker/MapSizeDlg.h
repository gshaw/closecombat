#if !defined(AFX_MAPSIZEDLG_H__2905198A_EF45_11D1_BD15_00A024CE2BB5__INCLUDED_)
#define AFX_MAPSIZEDLG_H__2905198A_EF45_11D1_BD15_00A024CE2BB5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MapSizeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMapSizeDlg dialog

class CMapSizeDlg : public CDialog
{
// Construction
public:
	CMapSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMapSizeDlg)
	enum { IDD = IDD_MAPSIZE };
	CSpinButtonCtrl	m_WidthSpinner;
	CSpinButtonCtrl	m_HeightSpinner;
	int		m_Height;
	int		m_Width;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMapSizeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPSIZEDLG_H__2905198A_EF45_11D1_BD15_00A024CE2BB5__INCLUDED_)
