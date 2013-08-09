#ifndef __UTIL_H
#define __UTIL_H

// text parsing helper functions
char*		ReadFile(const CString& PathName);
int			GetColumnInt(const char* p, int col);
CString		GetColumnStr(const char* p, int col);
const char* NextLine(const char* p);
const char* FindChar(const char* p, char ch);
int			CountRowsTo(const char* p, char ch);

// verify that specified folder is the close combat 2 data folder
bool		CC2_IsDataFolderValid(const CString& FolderPath);

// simple image io helpers
void		TgaWrite(const CString& PathName, CDIB* pImage);
CDIB*		TgaRead(const CString& PathName);

typedef struct
{
    unsigned char b,g,r,a;
} ARGB;

typedef struct
{
    int  width;
    int  height;
    int  bpp;
    int  startcolor;
    int  numcolors;
    ARGB palette[256];
} TGAINFO;

unsigned int GetBigEndian(void* pBuffer, int count);

#endif //__UTIL_H
