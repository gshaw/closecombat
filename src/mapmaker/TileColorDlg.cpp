// TileColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "TileColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTileColorDlg dialog


CTileColorDlg::CTileColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTileColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTileColorDlg)
	//}}AFX_DATA_INIT
}

void CTileColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTileColorDlg)
	DDX_Control(pDX, IDC_TYPE, m_TileCtrl);
	DDX_Control(pDX, IDC_COLOR, m_ColorCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTileColorDlg, CDialog)
	//{{AFX_MSG_MAP(CTileColorDlg)
	ON_CBN_SELCHANGE(IDC_TYPE, OnTileChange)
	//}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK, OnColorChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTileColorDlg message handlers

BOOL CTileColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int TileCount = g_App.m_Elements.GetSize();
	for (int i = 0; i < TileCount; i++)
	{
		m_TileCtrl.AddString(g_App.m_Elements[i].Name);
	}

	m_TileCtrl.SetCurSel(0);
	m_ColorCtrl.SetColour(g_App.m_Elements[0].Color);
	
	return TRUE;
}

void CTileColorDlg::OnTileChange() 
{
	int TileIndex = m_TileCtrl.GetCurSel();
	m_ColorCtrl.SetColour(g_App.m_Elements[TileIndex].Color);
}

LONG CTileColorDlg::OnColorChange(UINT /*lParam*/, LONG /*wParam*/)
{
	int TileIndex = m_TileCtrl.GetCurSel();
	g_App.m_Elements[TileIndex].Color = m_ColorCtrl.GetColour();
	g_App.InvalidateAllViews();
    return TRUE;
}
