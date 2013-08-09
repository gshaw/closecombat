// (c) 1997 Roger Onslow
// ColorDialogEx.cpp - auto load/save of custom colors CColorDialog extension

#include "stdafx.h"
#include "ColorDialogEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////
//
// CColorDialogEx

#define Inherited	CColorDialog

IMPLEMENT_DYNCREATE(CColorDialogEx, Inherited)

BEGIN_MESSAGE_MAP(CColorDialogEx, Inherited)
	//{{AFX_MSG_MAP(CColorDialogEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

bool	 CColorDialogEx::m_InitCustomColors = true;
COLORREF CColorDialogEx::m_CustomColors[k_CustomColorCount];
COLORREF CColorDialogEx::m_LastCustomColors[k_CustomColorCount];

#define k_Section	_T("Custom Colors")

void CColorDialogEx::InitCustomColors()
{
	for (int i = 0; i < k_CustomColorCount; i++)
	{
		CString entry; 
		entry.Format("%d",i);
		COLORREF cr = ::AfxGetApp()->GetProfileInt(k_Section, entry, RGB(255,255,255));

		m_LastCustomColors[i] = cr;
		m_CustomColors[i] = cr;
			
	}
	m_InitCustomColors = false;
}

void CColorDialogEx::SaveCustomColors()
{
     for (int i = 0; i < k_CustomColorCount; i++)
	 {
          if (m_LastCustomColors[i] != m_CustomColors[i])
		  {
               CString entry; 
			   entry.Format("%d",i);
               if (m_CustomColors[i] == RGB(255,255,255))
			   {
                    ::AfxGetApp()->WriteProfileString(k_Section,entry,NULL);
               }
			   else
			   {
                    ::AfxGetApp()->WriteProfileInt(k_Section, entry,m_CustomColors[i]);
               }               
			   m_LastCustomColors[i] = m_CustomColors[i];          
		  }
     }
}

CColorDialogEx::CColorDialogEx(COLORREF cr, DWORD Flags, CWnd* pParentWnd) : 
	Inherited(cr,Flags,pParentWnd)
{
     if (m_InitCustomColors)
	 {
          InitCustomColors();
     }
     m_cc.lpCustColors = m_CustomColors;
}

int CColorDialogEx::DoModal()
{
     int code = Inherited::DoModal();
	 if (code == IDOK)
		SaveCustomColors();
     return code;
}
