#include "StdAfx.h"
#include "FolderDialog.h"
#include "shlobj.h"

///////////////////////////////////////////////////////////////////
// CFolderDialog

CFolderDialog::CFolderDialog(LPCTSTR pPrompt, CWnd* pParent) :
    m_Prompt(pPrompt ? pPrompt : _T("Select a Folder"))
{
    m_pParent = pParent;
}

CString CFolderDialog::GetFolderPath() const
{
    return m_FolderPath;
}

int CFolderDialog::DoModal()
{
    int Result = IDCANCEL;

    // Work's only if we're 95 capable - part of the new shell so it might work under nt4
    LPMALLOC pMalloc;

    // get the shell's default allocator
    if (SHGetMalloc(&pMalloc) == NOERROR)
    {
        BROWSEINFO BI;
        char* pBuffer = m_FolderPath.GetBuffer(MAX_PATH);
        LPITEMIDLIST pIDList;

        // Get help on BROWSEINFO struct - it's got all the bit settings
        BI.hwndOwner      = m_pParent ? m_pParent->GetSafeHwnd() : NULL;
        BI.pidlRoot       = NULL;
        BI.pszDisplayName = pBuffer;
        BI.lpszTitle      = m_Prompt;
        BI.ulFlags        = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
        BI.lpfn           = NULL;
        BI.lParam         = 0;

        // issue dialog box
        if ((pIDList = SHBrowseForFolder(&BI)) != NULL)
        {
            // get path from selection
            if (SHGetPathFromIDList(pIDList, pBuffer))
            {
                // at this point pBuffer contains the selected path
                TRACE("folder selected: '%s'\n", (const char*)m_FolderPath);
                Result = IDOK;
            }

            // Free the PIDL allocated by SHBrowseForFolder
            pMalloc->Free(pIDList);
        }

        // release string buffer
        m_FolderPath.ReleaseBuffer();

        // release the shell's allocator
        pMalloc->Release();
    }

    return Result;
}
