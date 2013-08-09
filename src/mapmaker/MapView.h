// MapView.h : interface of the CMapView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __MAPVIEW_H
#define __MAPVIEW_H

#define VIEW_MODE_GRAPHICS	0
#define VIEW_MODE_TILES		1
#define VIEW_MODE_ELEVATION	2

#define GRID_MODE_OFF		0
#define GRID_MODE_MAJOR		1

class CMapView : public CScrollView
{
protected: // create from serialization only
	CMapView();
	DECLARE_DYNCREATE(CMapView)

// Attributes
public:
	CMapDoc* GetDocument();

// Operations
public:

// Overrides
	//{{AFX_VIRTUAL(CMapView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void DrawTile(CDC* pDC, int x, int y, int xe, int ye, const CTile& Tile);
	void DrawMegaTile(CDC* pDC, int x, int y);

	int m_ViewMode;		// VIEW_MODE_xxx
	int m_GridMode;		// GRID_MODE_xxx

	bool m_Tracking;
	bool m_HandDown;

// Generated message map functions
protected:
	//{{AFX_MSG(CMapView)
	afx_msg void OnViewElevation();
	afx_msg void OnUpdateViewElevation(CCmdUI* pCmdUI);
	afx_msg void OnViewGraphics();
	afx_msg void OnUpdateViewGraphics(CCmdUI* pCmdUI);
	afx_msg void OnViewTiles();
	afx_msg void OnUpdateViewTiles(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewGrid();
	afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MapView.cpp
inline CMapDoc* CMapView::GetDocument()
   { return (CMapDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}


#endif //__MAPVIEW_H
