// MapSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "App.h"
#include "MapSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapSizeDlg dialog


CMapSizeDlg::CMapSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapSizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapSizeDlg)
	m_Height = 0;
	m_Width = 0;
	//}}AFX_DATA_INIT
}


void CMapSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapSizeDlg)
	DDX_Control(pDX, IDC_WIDTH_SPINNER, m_WidthSpinner);
	DDX_Control(pDX, IDC_HEIGHT_SPINNER, m_HeightSpinner);
	DDX_Text(pDX, IDC_HEIGHT, m_Height);
	DDV_MinMaxInt(pDX, m_Height, 0, 12000);
	DDX_Text(pDX, IDC_WIDTH, m_Width);
	DDV_MinMaxInt(pDX, m_Width, 0, 12000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapSizeDlg, CDialog)
	//{{AFX_MSG_MAP(CMapSizeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapSizeDlg message handlers

BOOL CMapSizeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UDACCEL spindata;
	spindata.nSec = 0;
	spindata.nInc = 120;

	m_WidthSpinner.SetRange(0,12000);
	m_WidthSpinner.SetAccel(1, &spindata);

	m_HeightSpinner.SetRange(0,12000);
	m_HeightSpinner.SetAccel(1, &spindata);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
