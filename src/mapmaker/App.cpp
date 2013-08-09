// MapEdit.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "App.h"

#include "MainFrame.h"
#include "ChildFrame.h"
#include "MapDoc.h"
#include "MapView.h"

#include "FolderDialog.h"
#include "Util.h"
#include "ColorDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////
// Utility Functions

/////////////////////////////////////////////////////////////////////////////
// CMapMaker

CMapMaker g_App;

#define Inherited   CWinApp

BEGIN_MESSAGE_MAP(CMapMaker, Inherited)
    //{{AFX_MSG_MAP(CMapMaker)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TERRAIN_COLOR, OnTerrainColor)
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_FILE_NEW, Inherited::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, Inherited::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapMaker construction

CMapMaker::CMapMaker()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMapMaker initialization

BOOL CMapMaker::InitInstance()
{
    // Change the registry key under which our settings are stored.
    SetRegistryKey(_T("CombatTools"));

    LoadStdProfileSettings();  // Load standard INI file options (including MRU)
    ReadProgramSettings();

    // check to see if Close Combat path is valid
    if (!CC2_IsDataFolderValid(m_CloseCombatPath))
    {
        CFolderDialog dlg("Select the Close Combat 2 Data Folder\nie, E:\\Data");
        if (dlg.DoModal() == IDOK)
        {
            m_CloseCombatPath = dlg.GetFolderPath();
        }
        else
        {
            return FALSE;
        }
    }

    // read map elements
    if (!ReadElements())
        return FALSE;

    // create element menu
    if (!CreateElementMenu())
        return FALSE;

    // Register document templates
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAPDOC,
		RUNTIME_CLASS(CMapDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMapView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);
	// DON'T display a new MDI child window during startup!!!
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}


int CMapMaker::ExitInstance()
{
    WriteProgramSettings();
    return Inherited::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes

CMainFrame* CMapMaker::GetMainFrame()
{
	ASSERT_VALID(m_pMainWnd);
	return (CMainFrame*)m_pMainWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CMapMaker implementation

void CMapMaker::ReadProgramSettings()
{
    m_CloseCombatPath = GetProfileString("General", "m_CloseCombatPath");

	// read color file from application foler
	// moved to read elements, since this function gets called before elements are read
}

void CMapMaker::WriteProgramSettings()
{
    WriteProfileString("General", "m_CloseCombatPath", m_CloseCombatPath);

	// write out color file from application folder
	try
	{
		CFile f;
		if (f.Open("tilecolr.bin", CFile::modeCreate|CFile::modeWrite))
		{
			int TileCount = m_Elements.GetSize();
			for (int i = 0; i < TileCount; i++)
			{
				COLORREF c = m_Elements[i].Color;
				f.Write(&c, 4);
			}
			f.Close();
		}
	}
	catch (CFileException* e)
	{
		e->Delete();
	}
}

// 46   Version
// Element          Cover               Protection              Hindrance               Block       Top In Elem HE  Soldier Move Rate           Vehicle Move Rate                   Bog Type Flags                          Bridge      Tile

// Type Type    Height  Prone   Low Med High    Prone   Low Med High    Prone   Low Med High    Height  All Protection  Prot Flag   Effect  Prone   Crouch  Standing    Truck   Arm Car Halftrack   Tracked Pushed  Chance  Wall    Building    Sunken   Crushable   Covered Bridge  Flammable   Dir Explode Type    Level   Rubble To   Crush To
// &
// Grass    0   1   150 1   1   1   10  0   0   0   15  0   0   0   0   0   0   1   2   20  12  10  4   3   3   3   5   0   0   0   0   0   0   0   0   -1  1   0   0   22  -1
// High Grass   1   2   250 200 100 1   10  0   0   0   60  40  20  0   0   0   0   4   2   30  15  15  6   5   4   4   10  0   0   0   0   1   0   0   1   -1  1   1   0   22  0
// Dirt 2   0   80  1   1   1   10  0   0   0   5   0   0   0   0   0   0   1   2   20  10  8   3   2   2   2   5   0   0   0   0   0   0   0   0   -1  1   0   0   22  -1

BOOL CMapMaker::ReadElements()
{
    BOOL ok = TRUE;

    // delete existing elements
    m_Elements.RemoveAll();

    // create pathname
    CString PathName(g_App.m_CloseCombatPath);
    PathName += _T("\\data\\base\\elements");

    // read elements
    char* pBuffer = ReadFile(PathName);
    if (pBuffer)
    {
        const char* p = pBuffer;    // p points to the current row being processed
        try
        {
            // skip line with 41 on it
            VERIFY(GetColumnInt(p, 0) == 46);   p = NextLine(p);
            p = FindChar(p, '&');               p = NextLine(p);

            // p now points to start of element data (see above)
            int NumRows = CountRowsTo(p, '#');
            m_Elements.SetSize(NumRows);

            int i = 0;  // element index
            while (*p != '#')
            {
                CElement& Element = m_Elements[i];
                Element.Name     = GetColumnStr(p, 0);
                Element.Type     = GetColumnInt(p, 1);
                Element.Height   = GetColumnInt(p, 2);
                Element.TileType = GetColumnInt(p, 38);

                Element.Color = g_ElementTypeColors[Element.TileType];

                p = NextLine(p);
                i++;
            }
        }
        catch (...)
        {
            CString str;
            str.Format("Could not parse elements file %s", PathName);
            AfxMessageBox(str);
            m_Elements.RemoveAll();

            ok = FALSE;
        }

        delete pBuffer;
    }
    else
    {
        CString str;
        str.Format("Could not read elements from %s", PathName);
        AfxMessageBox(str);

        ok = FALSE;
    }

	// read color file from application foler
	try
	{
		CFile f;
		if (f.Open("tilecolr.bin", CFile::modeRead))
		{
			int TileCount = m_Elements.GetSize();
			for (int i = 0; i < TileCount; i++)
			{
				COLORREF c;
				f.Read(&c, 4);
				m_Elements[i].Color = c;
			}
			f.Close();
		}
	}
	catch (CFileException* e)
	{
		e->Delete();
	}

    return ok;
}

BOOL CMapMaker::CreateElementMenu()
{
    BOOL ok = TRUE;

    return ok;
}

/////////////////////////////////////////////////////////////////////////////
// CMapMaker operations

void CMapMaker::PrintStatus(const CString& Text, int Pane, bool Update)
{
    if (g_App.m_pMainWnd)
    {
        ASSERT_VALID(m_pMainWnd);
        ((CMainFrame*)m_pMainWnd)->m_StatusBar.SetPaneText(Pane, Text);

        if (Update || Pane == 0)
            ((CMainFrame*)m_pMainWnd)->m_StatusBar.UpdateWindow();
    }
}


/////////////////////////////////////////////////////////////////////////////
// CMapMaker commands

void CMapMaker::OnAppAbout()
{
    AfxMessageBox("Close Combat 2 Map Maker");
}

void CMapMaker::InvalidateAllViews()
{
	// invalidate all documents views

	POSITION pos = GetFirstDocTemplatePosition();
	while (pos)
	{
		CDocTemplate* pTemplate = GetNextDocTemplate(pos);
		POSITION docpos = pTemplate->GetFirstDocPosition();
		while (docpos)
		{
			CDocument* pDoc = pTemplate->GetNextDoc(docpos);
			pDoc->UpdateAllViews(NULL);
		}
	}
}

extern int g_Tile;

void CMapMaker::OnTerrainColor() 
{
	CColorDialogEx dlg(m_Elements[g_Tile].Color, CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		m_Elements[g_Tile].Color = dlg.GetColor();
		InvalidateAllViews();
	}
}
