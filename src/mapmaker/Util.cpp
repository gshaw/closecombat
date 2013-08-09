#include "StdAfx.h"
#include "Util.h"

bool CC2_IsDataFolderValid(const CString& FolderPath)
{
    // return true if FolderPath is a valid folder path to the close combat 2 game folder
    bool ok = false;

    // look for data\base\elements
    CString PathName(FolderPath);
    PathName += _T("\\data\\base\\elements");

    CFileStatus Status;
    if (CFile::GetStatus(PathName, Status))
    {
        ok = true;
    }
    return ok;
}


char* ReadFile(const CString& PathName)
{
    char* pBuffer = NULL;

    try
    {
        CFile File(PathName, CFile::modeRead);
        int BufferLen = File.GetLength();
        pBuffer = new char[BufferLen+1];
        ZeroMemory(pBuffer, BufferLen+1);
        File.Read(pBuffer, BufferLen);
    }
    catch (...)
    {
        if (pBuffer)
        {
            delete pBuffer;
            pBuffer = NULL;
        }
    }
    return pBuffer;
}

int GetColumnInt(const char* p, int col)
{
    // find the int at the col'th tab stop
    // if not a valid int throw an exception

    ASSERT(p != NULL);

    while (col > 0)
    {
        p++;

        // check for column out of range
        if ((*p == '\0') || (*p == '\r') || (*p == '\n'))
        {
            TRACE("could not find column %d.  Found new line instead.\n", col);
            throw;
        }

        // check for start of comment
        if (*p == '\\')
        {
            if (*(p+1) == '\\')
            {
                TRACE("could not find column %d.  Found start of comment instead.\n", col);
                throw;
            }
        }

        // check for tab stop
        if (*p == '\t')
            col--;
    }

    // p should now point to an int
    int i = 0;
    if (sscanf(p, "%d", &i) != 1)
    {
        TRACE("could not parse an int.  sscanf return bad value.\n");
        throw;
    }

    return i;
}

CString GetColumnStr(const char* p, int col)
{
    // find the int at the col'th tab stop
    // if not a valid int throw an exception

    ASSERT(p != NULL);

    while (col > 0)
    {
        p++;

        // check for column out of range
        if ((*p == '\0') || (*p == '\r') || (*p == '\n'))
        {
            TRACE("could not find column %d.  Found new line instead.\n", col);
            throw;
        }

        // check for start of comment
        if (*p == '\\')
        {
            if (*(p+1) == '\\')
            {
                TRACE("could not find column %d.  Found start of comment instead.\n", col);
                throw;
            }
        }

        // check for tab stop
        if (*p == '\t')
            col--;
    }

    // p should now point to an int
    char str[128];
    char* s = str;
    while ((*p != '\r') && (*p != '\n') && (*p != '\t'))
        *s++ = *p++;
    *s = NULL;

    return CString(str);
}

const char* NextLine(const char* p)
{
    ASSERT(p != NULL);

    while ((*p != '\0') && (*p != '\r') && (*p != '\n'))
        p++;

    ASSERT(*p != '\0');

    while ((*p == '\r') || (*p == '\n'))
        p++;

    return p;
}

const char* FindChar(const char* p, char ch)
{
    ASSERT(p != NULL);

    while ((*p != '\0') && (*p != ch))
        p++;
    return p;
}

int CountRowsTo(const char* p, char ch)
{
    int NumRows = 0;
    while (*p != ch)
    {
        p = NextLine(p);
        NumRows++;
    }
    return NumRows;
}

void TgaWrite(const CString& PathName, CDIB* pImage)
{
    TGAINFO info;
    ZeroMemory(&info, sizeof(TGAINFO));

    info.width       = pImage->GetWidth();
    info.height      = pImage->GetHeight();

    if (pImage->GetBpp() > 8)
    {
        info.bpp         = 32;
    }
    else
    {
        info.bpp         = 8;
        info.startcolor = 0;
        info.numcolors  = 256;

        RGBQUAD* pRGB = pImage->GetColorTable();
        for (int i = 0; i < 256; i++)
        {
            info.palette[i].a = 255;
            info.palette[i].r = pRGB[i].rgbRed;
            info.palette[i].g = pRGB[i].rgbGreen;
            info.palette[i].b = pRGB[i].rgbBlue;
        }
    }

    char* pathname = (char*)((const char*)PathName);

	// TODO: write TGA to pathname

}

CDIB* TgaRead(const CString& PathName)
{
    CDIB* pImage = NULL;

    char* pathname = (char*)((const char*)PathName);

	// TODO: read TGA image into CDIB

    return pImage;
}

unsigned int GetBigEndian(void* pBuffer, int count)
{
    unsigned char* p = (unsigned char*) pBuffer;
    unsigned int value;

    value = 0;
    while (count--) 
	{
        value = (value<<8) + ((*p++));
    }
    return value;
}
