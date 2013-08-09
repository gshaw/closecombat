// MapView.cpp : implementation of the CMapView class
//

#include "stdafx.h"
#include "App.h"

#include "MapDoc.h"
#include "MapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapView

#define Inherited   CScrollView
IMPLEMENT_DYNCREATE(CMapView, Inherited)

BEGIN_MESSAGE_MAP(CMapView, Inherited)
    //{{AFX_MSG_MAP(CMapView)
    ON_COMMAND(ID_VIEW_ELEVATION, OnViewElevation)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ELEVATION, OnUpdateViewElevation)
    ON_COMMAND(ID_VIEW_GRAPHICS, OnViewGraphics)
    ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPHICS, OnUpdateViewGraphics)
    ON_COMMAND(ID_VIEW_TILES, OnViewTiles)
    ON_UPDATE_COMMAND_UI(ID_VIEW_TILES, OnUpdateViewTiles)
    ON_WM_CREATE()
    ON_WM_MOUSEMOVE()
    ON_COMMAND(ID_VIEW_GRID, OnViewGrid)
    ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, OnUpdateViewGrid)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_SETCURSOR()
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapView construction/destruction

CMapView::CMapView()
{
    m_ViewMode = VIEW_MODE_GRAPHICS;
    m_GridMode = GRID_MODE_MAJOR;

    m_Tracking = false;

    m_HandDown = false;
}

CMapView::~CMapView()
{
}

int CMapView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (Inherited::OnCreate(lpCreateStruct) == -1)
        return -1;

    // strip unneccssary redraw bits (Prosise, VC DEVCON Orlando, p170 in session notes)
    DWORD Style = ::GetClassLong(m_hWnd, GCL_STYLE);
    ::SetClassLong(m_hWnd, GCL_STYLE, Style & ~(CS_HREDRAW | CS_VREDRAW));

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMapView drawing

void CMapView::DrawTile(CDC* pDC, int x, int y, int xe, int ye, const CTile& Tile)
{
    CMapDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    int e = ye * 4 + xe;

    COLORREF cr = g_App.m_Elements[Tile.E[e]].Color;

    pDC->FillSolidRect((x*40)+(xe*10),(y*40)+(ye*10), 10,10, cr);
}

void CMapView::DrawMegaTile(CDC* pDC, int x, int y)
{
    CMapDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    int r = y * pDoc->GetWidth() + x;
    CTile Tile = pDoc->GetTile(r);

    if (m_ViewMode == VIEW_MODE_TILES)
    {
        for (int ye = 0; ye < 4; ye++)
        {
            for (int xe = 0; xe < 4; xe++)
            {
                DrawTile(pDC, x,y,xe,ye, Tile);
            }
        }
    }
    else
    {
        ASSERT(m_ViewMode == VIEW_MODE_ELEVATION);

        BYTE v = (BYTE)(Tile.Elevation);
        v = (BYTE)min(255, v*5); // make difference in color more noticable
        pDC->FillSolidRect(x*40, y*40, 40,40, RGB(v,v,v));
    }
}

void CMapView::OnDraw(CDC* pDC)
{
    CMapDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    int h = pDoc->GetHeight();
    int w = pDoc->GetWidth();

    CRect r;
    pDC->GetClipBox(&r);
    pDC->FillSolidRect(r, GetSysColor(COLOR_3DFACE));

    if (w > 0 && h > 0)
    {
        // calculate the bounding box that is needed to be draw
        CRect r;
        pDC->GetClipBox(&r);
        int ys = r.top    / 40;
        int ye = r.bottom / 40;
        int xs = r.left   / 40;
        int xe = r.right  / 40;
        ye = min(ye+1,h);
        xe = min(xe+1,w);

        if (m_ViewMode == VIEW_MODE_GRAPHICS)
        {
			if (pDoc->m_pMapImage)
			{
				pDC->SetStretchBltMode(COLORONCOLOR);
				CRect r(0,0,pDoc->m_pMapImage->GetWidth(), pDoc->m_pMapImage->GetHeight());
				pDoc->m_pMapImage->Draw(pDC, r,r);
			}
        }
        else
        {
            for (int y = ys; y < ye; y++)
            {
                for (int x = xs; x < xe; x++)
                {
                    DrawMegaTile(pDC, x,y);
                }
            }
        }

        if (m_GridMode)
        {
            // draw major grid lines
            {
                CPen Pen;
                Pen.CreateStockObject(BLACK_PEN);
                CPen* pOldPen = pDC->SelectObject(&Pen);
                for (int y = ys; y <= ye; y++)
                {
                    pDC->MoveTo(xs*40,y*40);
                    pDC->LineTo(xe*40,y*40);

                }
                for (int x = xs; x <= xe; x++)
                {
                    pDC->MoveTo(x*40,ys*40);
                    pDC->LineTo(x*40,ye*40);
                }
                pDC->SelectObject(pOldPen);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CMapView diagnostics

#ifdef _DEBUG
void CMapView::AssertValid() const
{
    Inherited::AssertValid();
}

void CMapView::Dump(CDumpContext& dc) const
{
    Inherited::Dump(dc);
}

CMapDoc* CMapView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapDoc)));
    return (CMapDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapView message handlers

void CMapView::OnInitialUpdate()
{
    Inherited::OnInitialUpdate();

    CMapDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    int h = pDoc->GetHeight();
    int w = pDoc->GetWidth();

    SetScrollSizes(MM_TEXT, CSize(w*40, h*40));
}

////////////////////////////////////////////////////////////////////////////////
// view menu handing

void CMapView::OnViewElevation()
{
    m_ViewMode = VIEW_MODE_ELEVATION;
    Invalidate(FALSE);
}

void CMapView::OnUpdateViewElevation(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
    pCmdUI->SetRadio(m_ViewMode == VIEW_MODE_ELEVATION);
}

void CMapView::OnViewGraphics()
{
    m_ViewMode = VIEW_MODE_GRAPHICS;
    Invalidate(FALSE);
}

void CMapView::OnUpdateViewGraphics(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
    pCmdUI->SetRadio(m_ViewMode == VIEW_MODE_GRAPHICS);
}

void CMapView::OnViewTiles()
{
    m_ViewMode = VIEW_MODE_TILES;
    Invalidate(FALSE);
}

void CMapView::OnUpdateViewTiles(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
    pCmdUI->SetRadio(m_ViewMode == VIEW_MODE_TILES);
}


////////////////////////////////////////////////////////////////////////////////
// CMapView grid support

void CMapView::OnViewGrid()
{
    if (m_GridMode)
        m_GridMode = GRID_MODE_OFF;
    else
        m_GridMode = GRID_MODE_MAJOR;
    Invalidate(FALSE);
}

void CMapView::OnUpdateViewGrid(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
    pCmdUI->SetCheck(m_GridMode == GRID_MODE_MAJOR);
}

////////////////////////////////////////////////////////////////////////////////
// mouse support

extern int g_Tile;
extern int g_Elevation;

static CPoint m_LastPoint;

void CMapView::OnMouseMove(UINT, CPoint point)
{
    if (m_HandDown && m_Tracking)
    {
        CPoint Delta = m_LastPoint - point;
        CPoint NewPoint = GetScrollPosition() + Delta;
        ScrollToPosition(NewPoint);
        m_LastPoint = point;
    }
    else
    {
        point += GetScrollPosition();

        int x  = point.x / 40;
        int y  = point.y / 40;
        int xe = (point.x % 40) / 10;
        int ye = (point.y % 40) / 10;

        CMapDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        int w = pDoc->GetWidth();
        int h = pDoc->GetHeight();

        CString str;
        if (w > 0 && h > 0 && x >= 0 && y >= 0 && x < w && y < h && xe >= 0 && xe < 4 && ye >= 0 && ye < 4)
        {
            int r = y*w + x;
            int e = ye*4 + xe;

            CTile Tile = pDoc->GetTile(r);
            int Elevation = Tile.Elevation;
            CElement Element = g_App.m_Elements[Tile.E[e]];

            str.Format("%s (%d)", Element.Name, Element.Type);
            g_App.PrintStatus(str,1);

            str.Format("%dm", Elevation);
            g_App.PrintStatus(str,2);

            str.Format("%d,%d (%d,%d)", x,y, xe,ye);
            g_App.PrintStatus(str,3);

            str.Format("%d E%d", r,e);
            g_App.PrintStatus(str,4);

            if (m_Tracking)
            {
                if (m_ViewMode == VIEW_MODE_ELEVATION)
                {
                    pDoc->SetTileElevation(r, g_Elevation);
                    CString str;
                    str.Format("Change %d to %dm", r, g_Elevation);
                    g_App.PrintStatus(str);


                    CRect rect(CPoint(x*40, y*40), CSize(40,40));
                    rect.OffsetRect(-GetScrollPosition());
                    InvalidateRect(rect, FALSE);
                }
                else
                {
                    pDoc->SetTile(r,e, g_Tile);
                    CString str;
                    str.Format("Change %d [%d] to %s", r,e, g_App.m_Elements[g_Tile].Name);
                    g_App.PrintStatus(str);

                    CRect rect(CPoint(x*40 + xe*10, y*40 + ye*10), CSize(10,10));
                    rect.OffsetRect(-GetScrollPosition());
                    InvalidateRect(rect, FALSE);
                }
            }
        }
        else
        {
            str.Empty();
            g_App.PrintStatus(str,1);
            g_App.PrintStatus(str,2);
            g_App.PrintStatus(str,3);
            g_App.PrintStatus(str,4);
        }
        str.Format("%d,%d", point.x,point.y);
        g_App.PrintStatus(str,5,true);
    }
}

void CMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_HandDown)
    {
        m_Tracking = true;
        SetCapture();
        m_LastPoint = point;
    }
    else
    {
        CPoint pt = point + GetScrollPosition();

        int x  = pt.x / 40;
        int y  = pt.y / 40;
        int xe = (pt.x % 40) / 10;
        int ye = (pt.y % 40) / 10;

        CMapDoc* pDoc = GetDocument();
        ASSERT_VALID(pDoc);
        int w = pDoc->GetWidth();
        int h = pDoc->GetHeight();

        if (w > 0 && h > 0 && x >= 0 && y >= 0 && x < w && y < h && xe >= 0 && xe < 4 && ye >= 0 && ye < 4)
        {
            int r = y*w + x;
            int e = ye*4 + xe;

            if (m_ViewMode == VIEW_MODE_ELEVATION)
            {
                pDoc->SetTileElevation(r, g_Elevation);
                CString str;
                str.Format("Change %d to %dm", r, g_Elevation);
                g_App.PrintStatus(str);

                CRect rect(CPoint(x*40, y*40), CSize(40,40));
                rect.OffsetRect(-GetScrollPosition());
                InvalidateRect(rect, FALSE);
            }
            else
            {
                pDoc->SetTile(r,e, g_Tile);
                CString str;
                str.Format("Change %d [%d] to %s", r,e, g_App.m_Elements[g_Tile].Name);
                g_App.PrintStatus(str);

                CRect rect(CPoint(x*40 + xe*10, y*40 + ye*10), CSize(10,10));
                rect.OffsetRect(-GetScrollPosition());
                InvalidateRect(rect, FALSE);
            }

            m_Tracking = true;
            SetCapture();
        }
    }

    Inherited::OnLButtonDown(nFlags, point);
}

void CMapView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_Tracking)
    {
        ReleaseCapture();
        m_Tracking = false;
    }

    Inherited::OnLButtonUp(nFlags, point);
}


BOOL CMapView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (nHitTest == HTCLIENT)
    {
        if (m_HandDown)
            SetCursor(g_App.LoadCursor(IDC_HAND));
        else
            SetCursor(g_App.LoadCursor(IDC_BLOCK));

        return TRUE;
    }
    else
        return Inherited::OnSetCursor(pWnd, nHitTest, message);
}

void CMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == ' ' && !m_HandDown)
    {
        m_HandDown = true;
        SetCursor(g_App.LoadCursor(IDC_HAND));
    }

    Inherited::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMapView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == ' ')
    {
        m_HandDown = false;

        CRect rect;
        GetClientRect(&rect);
        ClientToScreen(&rect);
        CPoint pt;
        VERIFY(GetCursorPos(&pt));
        if (rect.PtInRect(pt))
            SetCursor(g_App.LoadCursor(IDC_BLOCK));
        else
            SetCursor(g_App.LoadStandardCursor(IDC_ARROW));
    }

    Inherited::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMapView::OnRButtonDown(UINT, CPoint point)
{
    CPoint pt = point + GetScrollPosition();

    int x  = pt.x / 40;
    int y  = pt.y / 40;
    int xe = (pt.x % 40) / 10;
    int ye = (pt.y % 40) / 10;

    CMapDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    int w = pDoc->GetWidth();
    int h = pDoc->GetHeight();

    if (w > 0 && h > 0 && x >= 0 && y >= 0 && x < w && y < h && xe >= 0 && xe < 4 && ye >= 0 && ye < 4)
    {
        int r = y*w + x;
        int e = ye*4 + xe;

        CTile Tile = pDoc->GetTile(r);
        g_Tile = Tile.E[e];
        g_Elevation = Tile.Elevation;

        CString str;
        str.Format("Select %s (%dm) from %d [%d]", g_App.m_Elements[g_Tile].Name, g_Elevation, r,e);
        g_App.PrintStatus(str);
    }
}

BOOL CMapView::OnEraseBkgnd(CDC*)
{
    return TRUE;
}
