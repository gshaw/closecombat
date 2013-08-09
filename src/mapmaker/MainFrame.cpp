#include "stdafx.h"
#include "App.h"

#include "MainFrame.h"
#include "WinPlace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int g_Category = 0;
static int g_Terrain = 0;

int g_Elevation = 0;
int g_Tile;

static CString g_CategoryNames[] =
{
    CString("Open"),
    CString("Concealing"),
    CString("Gulley"),
    CString("Trees"),
    CString("Bocage"),
    CString("Road"),
    CString("Bridge"),
    CString("Fortification"),
    CString("Bunker"),
    CString("Wall"),
    CString("Marsh"),
    CString("Wire"),
    CString("Water"),
    CString("Wood Building"),
    CString("Brick Building"),
    CString("Stone Building"),
    CString("Deep Water"),
    CString("Building Interior"),
    CString("Light Wall"),
    CString("Heavy Wall"),
    CString("Improved Position"),
    CString("Wood Bld L2"),
    CString("Wood Bld L3"),
    CString("Brick Bld L2"),
    CString("Brick Bld L3"),
    CString("Stone Bld L2"),
    CString("Stone Bld L3"),
    CString("Stone Bld L4"),
    CString("Interior L2"),
    CString("Interior L3"),
    CString("Interior L4")
};

#define NUMCATEGORIES   30
#define MAXTILES        200

static int g_TerrainToTile[MAXTILES];


////////////////////////////////////////////////////////////////////////////////
// CEditToolBar

const int k_CatagoryIndex = 10;
const int k_TerrainIndex = 12;
const int k_ElevationIndex = 16;
const int k_ElevationSpinnerIndex = 17;

bool CEditToolBar::Create(CMainFrame* pParent)
{
    ASSERT_VALID(pParent);

    if (!CToolBar::Create(pParent, WS_CHILD | WS_VISIBLE | /*CBRS_SIZE_DYNAMIC |*/ CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY) ||
        !LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        ASSERT(false);
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////

    // Create an 8-point MS Sans Serif font for the combo boxes
    CClientDC dc(this);
    int FontHeight = -((dc.GetDeviceCaps (LOGPIXELSY) * 8) / 72);
    m_ToolBarFont.CreateFont(FontHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");

    // mearsure font to get size of average character
    CFont* pOldFont = dc.SelectObject(&m_ToolBarFont);
    TEXTMETRIC tm;
    dc.GetTextMetrics(&tm);
    int CharWidth  = tm.tmAveCharWidth;
    int CharHeight = tm.tmHeight + tm.tmExternalLeading;
    dc.SelectObject(pOldFont);


    // create catagory list
    {
        SetButtonInfo(k_CatagoryIndex, ID_CATEGORY, TBBS_SEPARATOR, CharWidth*25);

        CRect rect;
        GetItemRect(k_CatagoryIndex, &rect);
        rect.bottom = rect.top + CharHeight*35;

        if (!m_CategoryList.Create(
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
            rect, this, ID_CATEGORY))
            return false;

        m_CategoryList.SetFont(&m_ToolBarFont);
        for (int i = 0; i < NUMCATEGORIES; i++)
        {
            m_CategoryList.AddString(g_CategoryNames[i]);
        }
        m_CategoryList.SetCurSel(g_Category);
    }

    // create terrain list
    {
        SetButtonInfo(k_TerrainIndex, ID_TERRAIN, TBBS_SEPARATOR, CharWidth*30);

        CRect rect;
        GetItemRect(k_TerrainIndex, &rect);
        rect.bottom = rect.top + CharHeight*20;

        if (!m_TerrainList.Create(
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
            rect, this, ID_TERRAIN))
            return false;

        m_TerrainList.SetFont(&m_ToolBarFont);
        for (int i = 0; i < MAXTILES; i++)
            g_TerrainToTile[i] = -1;
        int j=0;
        for (i = 0; i < g_App.m_Elements.GetSize(); i++)
        {
            if (g_App.m_Elements[i].TileType == g_Category)
            {
                g_TerrainToTile[j++] = i;
                m_TerrainList.AddString(g_App.m_Elements[i].Name);

            }
        }
        g_Terrain = 0;
        m_TerrainList.SetCurSel(g_Terrain);
    }

    // create elevation edit
    {
        SetButtonInfo(k_ElevationIndex, ID_ELEVATION, TBBS_SEPARATOR, CharWidth*5);

        CRect rect;
        GetItemRect(k_ElevationIndex, &rect);
        rect.bottom -= 1;
        rect.top += 1;

        if (!m_ElevationEdit.Create(ES_NUMBER | WS_BORDER | WS_CHILD | WS_VISIBLE,
            rect, this, ID_ELEVATION))
            return false;
        m_ElevationEdit.SetFont(&m_ToolBarFont);
        g_Elevation = 20;
        CString str;
        str.Format("%d", g_Elevation);
        m_ElevationEdit.SetWindowText(str);

        SetButtonInfo(k_ElevationSpinnerIndex, ID_ELEVATION_SPINNER, TBBS_SEPARATOR, 20);
        GetItemRect(k_ElevationSpinnerIndex, &rect);
        rect.left += 3;
        rect.right += 3;
        if (!m_ElevationCtrl.Create(UDS_SETBUDDYINT|UDS_ARROWKEYS| WS_CHILD | WS_VISIBLE,
            rect, this, ID_ELEVATION_SPINNER))
            return false;
        m_ElevationCtrl.SetBuddy(&m_ElevationEdit);
        m_ElevationCtrl.SetRange(0,100);

    }

    SetWindowText(_T(""));
    EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
    pParent->DockControlBar(this, AFX_IDW_DOCKBAR_TOP);


    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP (CEditToolBar, CToolBar)
    ON_WM_CREATE ()
    ON_CBN_SELENDOK(ID_CATEGORY,        OnSelectCategory)
    ON_CBN_SELENDOK(ID_TERRAIN,         OnSelectTerrain)
    ON_EN_CHANGE(ID_ELEVATION,          OnChangeElevation)
    ON_UPDATE_COMMAND_UI(ID_TERRAIN,    OnTerrainUpdate)
    ON_UPDATE_COMMAND_UI(ID_ELEVATION,  OnElevationUpdate)
    ON_CBN_CLOSEUP(ID_CATEGORY,         OnCloseUp)
    ON_CBN_CLOSEUP(ID_TERRAIN,          OnCloseUp)
END_MESSAGE_MAP ()

////////////////////////////////////////////////////////////////////////////////
// Message Handlers

void CEditToolBar::OnSelectCategory()
{
    int Category = m_CategoryList.GetCurSel();
    if (Category != g_Category)
    {
        g_Category = Category;
        TRACE("g_Category set to: %d\n", g_Category);

        m_TerrainList.ResetContent();

        for (int i = 0; i < MAXTILES; i++)
            g_TerrainToTile[i] = -1;
        int j=0;
        for (i = 0; i < g_App.m_Elements.GetSize(); i++)
        {
            if (g_App.m_Elements[i].TileType == g_Category)
            {
                g_TerrainToTile[j++] = i;
                m_TerrainList.AddString(g_App.m_Elements[i].Name);

            }
        }
        g_Terrain = 0;
        m_TerrainList.SetCurSel(g_Terrain);
        g_Tile = g_TerrainToTile[g_Terrain];
    }
}

void CEditToolBar::OnSelectTerrain()
{
    int Terrain = m_TerrainList.GetCurSel();
    g_Tile = g_TerrainToTile[Terrain];
    ASSERT(g_Tile >= 0 && g_Tile < g_App.m_Elements.GetSize());
    TRACE("g_Tile set to: %d\n", g_Tile);
}

void CEditToolBar::OnTerrainUpdate(CCmdUI* /*pCmdUI*/)
{
    if (GetFocus() == &m_TerrainList)
        return;

    int Tile = g_TerrainToTile[m_TerrainList.GetCurSel()];
    ASSERT(Tile >= 0 && Tile < g_App.m_Elements.GetSize());

    if (g_Tile != Tile)
    {
        g_Category = g_App.m_Elements[g_Tile].TileType;
        m_CategoryList.SetCurSel(g_Category);
        TRACE("g_Category set to: %d\n", g_Category);

        m_TerrainList.ResetContent();
        for (int i = 0; i < MAXTILES; i++)
            g_TerrainToTile[i] = -1;
        int j=0;
        for (i = 0; i < g_App.m_Elements.GetSize(); i++)
        {
            if (g_App.m_Elements[i].TileType == g_Category)
            {
                if (i == g_Tile)
                    g_Terrain = j;

                g_TerrainToTile[j++] = i;
                m_TerrainList.AddString(g_App.m_Elements[i].Name);
            }
        }
        m_TerrainList.SetCurSel(g_Terrain);
    }
}

void CEditToolBar::OnChangeElevation()
{
    CString str;
    m_ElevationEdit.GetWindowText(str);
    g_Elevation = atoi(str);
    ASSERT(g_Elevation >= 0);
}

void CEditToolBar::OnElevationUpdate(CCmdUI* /*pCmdUI*/)
{
    if (GetFocus() == &m_TerrainList)
        return;

    CString str;
    m_ElevationEdit.GetWindowText(str);
    int Elevation = atoi(str);

    if (Elevation != g_Elevation)
    {
        str.Format("%d", g_Elevation);
        m_ElevationEdit.SetWindowText(str);
    }
}


void CEditToolBar::OnCloseUp()
{
	g_App.m_pMainWnd->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

#define Inherited   CMDIFrameWnd
IMPLEMENT_DYNCREATE(CMainFrame, Inherited)

BEGIN_MESSAGE_MAP(CMainFrame, Inherited)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

static UINT Indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_DESCRIPTION,
    ID_INDICATOR_ELEVATION,
    ID_INDICATOR_TILEXY,
    ID_INDICATOR_ROW_E,
    ID_INDICATOR_MOUSE
};

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (Inherited::OnCreate(lpCreateStruct) == -1)
        return -1;

    // create status bar
    if (!m_StatusBar.Create(this) || !m_StatusBar.SetIndicators(Indicators, sizeof(Indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // enable docking after status bar is created
    EnableDocking(CBRS_ALIGN_ANY);

    // create standard tool bar
    if (!m_ToolBar.Create(this))
        return -1;

    // strip unneccssary redraw bits (Prosise, VC DEVCON Orlando, p170 in session notes)
    DWORD Style = ::GetClassLong(m_hWnd, GCL_STYLE);
    ::SetClassLong(m_hWnd, GCL_STYLE, Style & ~(CS_HREDRAW | CS_VREDRAW));

	// restore window placement - main window
	CWindowPlacement MainWindowPlacement;
	MainWindowPlacement.Restore(_T("Main Window"), this);

	// restore toolbar placement
	LoadBarState(_T("ToolBarSettings"));

    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnClose() 
{
	// save toolbar's window placement, see Prosise p728 for why we do it here instead
	SaveBarState(_T("ToolBarSettings"));

	// save window placement - main window
	CWindowPlacement MainWindowPlacement;
	MainWindowPlacement.Save(_T("Main Window"), this);
	
	Inherited::OnClose();
}
