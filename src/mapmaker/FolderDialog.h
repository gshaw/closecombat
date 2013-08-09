#ifndef __FOLDERDIALOG_H
#define __FOLDERDIALOG_H

class CFolderDialog
{
// Construction
public:
	CFolderDialog(LPCTSTR pPrompt = NULL, CWnd* pParent = NULL);
	int DoModal();

	CString GetFolderPath() const;

// Implementation
protected:
	CString m_FolderPath;
	CString m_Prompt;
	CWnd*   m_pParent;
};

#endif //__FOLDERDIALOG_H
