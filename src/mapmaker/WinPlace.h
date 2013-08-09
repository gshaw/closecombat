/* adapted from Q&A C++ MSJ Mar 96, Paul DiLascia                        */

// CWindowPlacement reads and writes WINDOWPLACEMENT 
// from/to application profile

#pragma once

class CWindowPlacement : public tagWINDOWPLACEMENT
{
public:
	CWindowPlacement();
	~CWindowPlacement();
	
	// Read/write to app profile
	void GetProfileWP(const CString& KeyName);
	void WriteProfileWP(const CString& KeyName);

	// Save/restore window pos (from app profile)
	void Save(const CString& KeyName, CWnd* pWnd);
	BOOL Restore(const CString& KeyName, CWnd* pWnd);
};
