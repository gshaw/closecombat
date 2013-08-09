#ifndef __MAINFRM_H
#define __MAINFRM_H

class CMainFrame;

class CEditToolBar : public CToolBar  
{
public:
	bool Create(CMainFrame* pParent);

private:
	CFont			m_ToolBarFont;	// font used in toolbar controls

	CComboBox		m_CategoryList;
	CComboBox		m_TerrainList;
	CEdit			m_ElevationEdit;
	CSpinButtonCtrl	m_ElevationCtrl;

// message map functions
protected:
    afx_msg void OnSelectCategory();
    afx_msg void OnSelectTerrain();
	afx_msg void OnChangeElevation();
    
	afx_msg void OnCloseUp();

	afx_msg void OnCategoryUpdate(CCmdUI* pCmdUI);
	afx_msg void OnTerrainUpdate(CCmdUI* pCmdUI);
	afx_msg void OnElevationUpdate(CCmdUI* pCmdUI);
    DECLARE_MESSAGE_MAP()
};


class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNCREATE(CMainFrame)

public:
	CStatusBar	 m_StatusBar;
	CEditToolBar m_ToolBar;

protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__MAINFRM_H
