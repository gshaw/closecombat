#include "StdAfx.h"
#include "WinPlace.h"

CWindowPlacement::CWindowPlacement()
{
	// Note: "length" is inherited from WINDOWPLACEMENT
	length = sizeof(WINDOWPLACEMENT);
}

CWindowPlacement::~CWindowPlacement()
{
}

// Restore window placement from profile key
BOOL CWindowPlacement::Restore(const CString& KeyName, CWnd* pWnd)
{
	GetProfileWP(KeyName);
	
	// Only restore if window intersets the screen.
	CRect rcTemp;
	CRect rcScreen(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	if (!::IntersectRect(&rcTemp, &rcNormalPosition, &rcScreen))
		return FALSE;

	// if window is a dialog then use original size
	if (pWnd->IsKindOf(RUNTIME_CLASS(CDialog)))
	{
		WINDOWPLACEMENT wp;
		pWnd->GetWindowPlacement(&wp);

		int width  = (wp.rcNormalPosition.right  - wp.rcNormalPosition.left);
		int height = (wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);

		rcNormalPosition.right  = rcNormalPosition.left + width;
		rcNormalPosition.bottom = rcNormalPosition.top  + height;
	}

	// There seems to be a flaw in SetWindowPlacement if showCmd = SW_SHOWMINIMIZED  The
	// window will be restore minimized but will *think* it is normal preventing a restore
	// from happening.  This is very bad.  To get around this I will not save the state
	// of a window if the showCmd is minimized (see WriteProfileWP)
	pWnd->SetWindowPlacement(this);	// set placement
	return TRUE;
}

// Get window placement from profile
void CWindowPlacement::GetProfileWP(const CString& KeyName)
{
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);
	
	showCmd = pApp->GetProfileInt(KeyName, "wp.showCmd",  showCmd);
	flags   = pApp->GetProfileInt(KeyName, "wp.flags",    flags);
	
	ptMinPosition.x = pApp->GetProfileInt(KeyName, "wp.ptMinPosition.x", ptMinPosition.x);
	ptMinPosition.y = pApp->GetProfileInt(KeyName, "wp.ptMinPosition.y", ptMinPosition.y);
	ptMaxPosition.x = pApp->GetProfileInt(KeyName, "wp.ptMaxPosition.x", ptMaxPosition.x);
	ptMaxPosition.y = pApp->GetProfileInt(KeyName, "wp.ptMaxPosition.y", ptMaxPosition.y);
	
	RECT& rc  = rcNormalPosition; // alias to struct memeber
	rc.left	  = pApp->GetProfileInt(KeyName, "wp.left",	  rc.left);
	rc.right  = pApp->GetProfileInt(KeyName, "wp.right",  rc.right);
	rc.top	  = pApp->GetProfileInt(KeyName, "wp.top",    rc.top);
	rc.bottom = pApp->GetProfileInt(KeyName, "wp.bottom", rc.bottom);
}

// Save window placement in app profile
void CWindowPlacement::Save(const CString& KeyName, CWnd* pWnd)
{
	pWnd->GetWindowPlacement(this);

	// hiding windows do not seem to register as hiding
	if (!pWnd->IsWindowVisible())
		showCmd = SW_HIDE;

	WriteProfileWP(KeyName);
}

// Write window placement to app profile
void CWindowPlacement::WriteProfileWP(const CString& KeyName)
{
	CWinApp* pApp = AfxGetApp();
	ASSERT_VALID(pApp);

	// reason for prevent state save on min/max is in Restore method
	if (showCmd != SW_SHOWMINIMIZED && showCmd != SW_SHOWMAXIMIZED)
	{
		pApp->WriteProfileInt(KeyName, "wp.showCmd",		 showCmd);
		pApp->WriteProfileInt(KeyName, "wp.flags",			 flags);
		pApp->WriteProfileInt(KeyName, "wp.ptMinPosition.x", ptMinPosition.x);
		pApp->WriteProfileInt(KeyName, "wp.ptMinPosition.y", ptMinPosition.y);
		pApp->WriteProfileInt(KeyName, "wp.ptMaxPosition.x", ptMaxPosition.x);
		pApp->WriteProfileInt(KeyName, "wp.ptMaxPosition.y", ptMaxPosition.y);
		pApp->WriteProfileInt(KeyName, "wp.left",	         rcNormalPosition.left);
		pApp->WriteProfileInt(KeyName, "wp.right",           rcNormalPosition.right);
		pApp->WriteProfileInt(KeyName, "wp.top",             rcNormalPosition.top);
		pApp->WriteProfileInt(KeyName, "wp.bottom",          rcNormalPosition.bottom);
	}
}
