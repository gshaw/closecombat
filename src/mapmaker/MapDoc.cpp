// MapDoc.cpp : implementation of the CMapDoc class
//

#include "stdafx.h"
#include "App.h"

#include "MapDoc.h"
#include "Util.h"
#include "MapSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapDoc

#define Inherited   CDocument
IMPLEMENT_DYNCREATE(CMapDoc, Inherited)

BEGIN_MESSAGE_MAP(CMapDoc, Inherited)
    //{{AFX_MSG_MAP(CMapDoc)
    ON_COMMAND(ID_IMPORT_MAP, OnImportMap)
    ON_COMMAND(ID_EXPORT_MAP, OnExportMap)
    ON_COMMAND(ID_EXPORT_ELEVATION, OnExportElevation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapDoc construction/destruction

CMapDoc::CMapDoc()
{
    m_Width  = 0;
    m_Height = 0;

    m_pMapImage = NULL;
}

CMapDoc::~CMapDoc()
{
    if (m_pMapImage)
    {
        delete m_pMapImage;
        m_pMapImage = NULL;
    }
}


// 41
// 42 X Max
// 30 Y Max
// Idx  E0  E1  E2  E3  E4  E5  E6  E7  E8  E9  E10 E11 E12 E13 E14 E15 Elev
// &
// 0    8   0   0   0   8   8   0   0   8   8   0   0   0   8   8   8   29

BOOL CMapDoc::WriteMapTiles(const CString& PathName)
{
    int ok = TRUE;
    CString Buffer;
    CString str;

    // write version, width,height
    str.Format("%d\n%d\tX Max\n%d\tY Max\n", 41, m_Width, m_Height);
    Buffer += str;

    // write header, & (start of data marker)
    str.Format("Idx\tE0\tE1\tE2\tE3\tE4\tE5\tE6\tE7\tE8\tE9\tE10\tE11\tE12\tE13\tE14\tE15\tElev\n&\n");
    Buffer += str;

    // write tiles
    int NumTiles = m_Width*m_Height;
    for (int i = 0; i < NumTiles; i++)
    {
        str.Format("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            i,
            m_MapTiles[i].E[0],
            m_MapTiles[i].E[1],
            m_MapTiles[i].E[2],
            m_MapTiles[i].E[3],
            m_MapTiles[i].E[4],
            m_MapTiles[i].E[5],
            m_MapTiles[i].E[6],
            m_MapTiles[i].E[7],
            m_MapTiles[i].E[8],
            m_MapTiles[i].E[9],
            m_MapTiles[i].E[10],
            m_MapTiles[i].E[11],
            m_MapTiles[i].E[12],
            m_MapTiles[i].E[13],
            m_MapTiles[i].E[14],
            m_MapTiles[i].E[15],
            m_MapTiles[i].Elevation);
        Buffer += str;
    }

    // write # (EOF marker)
    str.Format("#\n\0");
    Buffer += str;

    // write map to disk
    try
    {
        CFile File(PathName, CFile::modeCreate | CFile::modeWrite);
        int BufferLen = Buffer.GetLength();
        void* pBuffer = Buffer.GetBuffer(BufferLen);
        File.Write(pBuffer, BufferLen);
        str.ReleaseBuffer();
        str.Empty();
    }
    catch (CFileException* e)
    {
        e->ReportError();
        e->Delete();
        ok = FALSE;
    }

    return ok;
}

BOOL CMapDoc::ReadMapTiles(const CString& PathName)
{
    BOOL ok = TRUE;

    // delete existing map
    m_MapTiles.RemoveAll();

    // read map
    char* pBuffer = ReadFile(PathName);
    if (pBuffer)
    {
        const char* p = pBuffer;    // p points to the current row being processed

        try
        {
            // skip line with 41 on it
            VERIFY(GetColumnInt(p, 0) == 41);   p = NextLine(p);
            m_Width  = GetColumnInt(p, 0);      p = NextLine(p);
            m_Height = GetColumnInt(p, 0);      p = NextLine(p);
            p = FindChar(p, '&');               p = NextLine(p);

            // p now points to start of map data (see above)
            int NumTiles = m_Width * m_Height;
            m_MapTiles.SetSize(NumTiles);

            for (int i = 0; i < NumTiles; i++)
            {
                CTile& Tile = m_MapTiles[i];

                Tile.Row = GetColumnInt(p, 0);;

                Tile.E[0]  = GetColumnInt(p, 1);
                Tile.E[1]  = GetColumnInt(p, 2);
                Tile.E[2]  = GetColumnInt(p, 3);
                Tile.E[3]  = GetColumnInt(p, 4);
                Tile.E[4]  = GetColumnInt(p, 5);
                Tile.E[5]  = GetColumnInt(p, 6);
                Tile.E[6]  = GetColumnInt(p, 7);
                Tile.E[7]  = GetColumnInt(p, 8);
                Tile.E[8]  = GetColumnInt(p, 9);
                Tile.E[9]  = GetColumnInt(p, 10);
                Tile.E[10] = GetColumnInt(p, 11);
                Tile.E[11] = GetColumnInt(p, 12);
                Tile.E[12] = GetColumnInt(p, 13);
                Tile.E[13] = GetColumnInt(p, 14);
                Tile.E[14] = GetColumnInt(p, 15);
                Tile.E[15] = GetColumnInt(p, 16);

                Tile.Elevation = GetColumnInt(p, 17);

                p = NextLine(p);
            }
        }
        catch (...)
        {
            CString str;
            str.Format("Could not parse map file %s", PathName);
            AfxMessageBox(str);

            m_MapTiles.RemoveAll();
            m_Width  = 0;
            m_Height = 0;
            ok = FALSE;
        }

        delete pBuffer;
    }
    else
    {
        CString str;
        str.Format("Could not read map file %s", PathName);
        AfxMessageBox(str);
        ok = FALSE;
    }
    return ok;
}

BOOL CMapDoc::ReadMapImage(const CString& PathName)
{
    BOOL ok = TRUE;

    char* pBuffer = ReadFile(PathName);
    if (pBuffer)
    {
        MAP_HEADER* buffer = (MAP_HEADER*)pBuffer;
        int w = GetBigEndian(buffer->width, 4);
        int h = GetBigEndian(buffer->height, 4);

        if (m_pMapImage)
            delete m_pMapImage;

        m_pMapImage = new CDIB;
        m_pMapImage->Create(w,h, 32);

        if (m_pMapImage)
        {
            unsigned short* src = (unsigned short*)((unsigned char*)buffer+16);
            ARGB* dst           = (ARGB*)(m_pMapImage->GetBase());

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    unsigned short p = *src;
                    unsigned char* s = (unsigned char*)src;

                    dst->a = 255; //(((*s)>>7)&1)*255;
                    dst->r = (unsigned char)(((*(s+0)>>2)&31)*255/31);
                    dst->g = (unsigned char)((((*(s+0)&3)<<3) | ((*(s+1)>>5)&7))*255/31);
                    dst->b = (unsigned char)((*(s+1)&31)*255/31);

                    dst++;
                    src++;
                }
            }
        }

        delete pBuffer;
    }

    return ok;
}

void CMapDoc::DeleteContents()
{
    // read map
    m_MapTiles.RemoveAll();
    m_Width  = 0;
    m_Height = 0;

    if (m_pMapImage)
    {
        delete m_pMapImage;
        m_pMapImage = NULL;
    }

    SetModifiedFlag(FALSE);
}

BOOL CMapDoc::OnNewDocument()
{
    if (!Inherited::OnNewDocument())
        return FALSE;

    // create a default map of grass
	CMapSizeDlg dlg;
	dlg.m_Width  = 7*40*3;
	dlg.m_Height = 7*40*3;
	if (dlg.DoModal() != IDOK)
		return FALSE;

    m_Width  = dlg.m_Width/40;
    m_Height = dlg.m_Height/40;

	if (m_Width < 0 || m_Width > 12000 || m_Height < 0 || m_Height > 12000)
	{
		AfxMessageBox("Invalid map size");
		return FALSE;
	}

    // fill in each tile with grass
    int NumTiles = m_Width * m_Height;
    m_MapTiles.RemoveAll();
    m_MapTiles.SetSize(NumTiles);
    for (int i = 0; i < NumTiles; i++)
    {
        CTile& Tile = m_MapTiles[i];

        Tile.Row = i;
        Tile.E[0]  = 0;
        Tile.E[1]  = 0;
        Tile.E[2]  = 0;
        Tile.E[3]  = 0;
        Tile.E[4]  = 0;
        Tile.E[5]  = 0;
        Tile.E[6]  = 0;
        Tile.E[7]  = 0;
        Tile.E[8]  = 0;
        Tile.E[9]  = 0;
        Tile.E[10] = 0;
        Tile.E[11] = 0;
        Tile.E[12] = 0;
        Tile.E[13] = 0;
        Tile.E[14] = 0;
        Tile.E[15] = 0;
        Tile.Elevation = 20;    // 20m
    }

    // create green bitmap
    {
        int w = m_Width*40;
        int h = m_Height*40;

        ASSERT(m_pMapImage == NULL);
        m_pMapImage = new CDIB;
        m_pMapImage->Create(w,h, 32);
        ASSERT(m_pMapImage);

        ARGB* dst = (ARGB*)(m_pMapImage->GetBase());
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                dst->a = 255; //(((*s)>>7)&1)*255;
                dst->r = 82;
                dst->g = 113;
                dst->b = 62;
                dst++;
            }
        }
    }

    return TRUE;
}

BOOL CMapDoc::OnOpenDocument(LPCTSTR pPathName)
{
    BOOL ok = TRUE;

    if (ReadMapTiles(pPathName))
    {
        // typical pPathName: "d:\\games\\cc2\\data\\maps\\map100"

        // get map number
        int MapNumber = 0;
        {
            const char* s = pPathName + strlen(pPathName) - 1;
            while (s > pPathName && isdigit(*s))
                s--;
            s++;

            sscanf(s, "%d", &MapNumber);
        }

        CString MapImagePathName;
        MapImagePathName.Format("%s\\graphics\\maps\\bgmap%d", g_App.m_CloseCombatPath, MapNumber);
        ok = ReadMapImage(MapImagePathName);
    }
    else
        ok = FALSE;

    return ok;
}

BOOL CMapDoc::OnSaveDocument(LPCTSTR pPathName)
{
    BOOL ok = TRUE;

    if (WriteMapTiles(pPathName))
    {
    }
    return ok;
}


////////////////////////////////////////////////////////////////////////////////
// attributes

int CMapDoc::GetWidth() const
{
    return m_Width;
}

int CMapDoc::GetHeight() const
{
    return m_Height;
}

CTile CMapDoc::GetTile(int Row) const
{
    return m_MapTiles[Row];
}

void CMapDoc::SetTile(int r, int e, int Tile)
{
    ASSERT(r < m_Width*m_Height);
    ASSERT(e >= 0 && e <= 15);
    ASSERT(Tile >= 0 && Tile < g_App.m_Elements.GetSize());

    m_MapTiles[r].E[e] = Tile;
    SetModifiedFlag();
}

void CMapDoc::SetTileElevation(int r, int Elevation)
{
    ASSERT(r < m_Width*m_Height);
    ASSERT(Elevation >= 0 && Elevation < 200);

    m_MapTiles[r].Elevation = Elevation;
    SetModifiedFlag();
}



////////////////////////////////////////////////////////////////////////////////
// Map Import/Export support

void CMapDoc::OnImportMap()
{
    CFileDialog dlg(TRUE, ".tga", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "TGA Files (*.tga)|*.tga|All Files (*.*)|*.*||");
    if (dlg.DoModal() == IDOK)
    {
        CDIB* pImage = TgaRead(dlg.GetPathName());

        if (pImage)
        {
            int w = pImage->GetWidth();
            int h = pImage->GetHeight();
            int bpp = pImage->GetBpp();

            bool ok = true;
            if (bpp <= 8)
            {
                AfxMessageBox("Imported Image is not correct bit depth, must be 16 bit or higher");
                ok = false;
            }

            if (w != m_Width*40 || h != m_Height*40)
            {
                CString str;
                str.Format("Imported Image is not correct size, must be %d x %d pixels", m_Width*40, m_Height*40);
                AfxMessageBox(str);
                ok = false;
            }

            if (ok)
            {
                // replace existing image with new image
                if (m_pMapImage)
                    delete m_pMapImage;
                m_pMapImage = pImage;
                SetModifiedFlag();
                UpdateAllViews(NULL);
            }
            else
            {
                delete pImage;
                pImage = NULL;
            }
        }
        else
        {
            CString str;
            str.Format("Could not open TGA file %d", dlg.GetPathName());
            AfxMessageBox(str);
        }
    }
}

void CMapDoc::OnExportMap()
{
    CFileDialog dlg(FALSE, ".tga", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "TGA Files (*.tga)|*.tga|All Files (*.*)|*.*||");
    if (dlg.DoModal() == IDOK)
    {
        // write image as a tga
        TgaWrite(dlg.GetPathName(), m_pMapImage);
    }
}


void CMapDoc::OnExportElevation()
{
    CFileDialog dlg(FALSE, ".tga", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "TGA Files (*.tga)|*.tga|All Files (*.*)|*.*||");
    if (dlg.DoModal() == IDOK)
    {
        // create an elevation image
        int w = m_Width;
        int h = m_Height;

        int i = 0;  // index into tile array

        CDIB Elevation;
        Elevation.Create(w,h, 8);

        int RowBytes = Elevation.GetRowBytes();
        for (int y = 0; y < h; y++)
        {
            BYTE* RowBuffer = Elevation.GetBase() + y*RowBytes;

            for (int x = 0; x < w; x++)
            {
                BYTE e = (BYTE)(m_MapTiles[i++].Elevation);
                RowBuffer[x] = e;
            }
        }

        // create grayscale color table
        ASSERT(Elevation.GetColorTableCount() >= 256);
        RGBQUAD* Palette = Elevation.GetColorTable();
        for (i = 0; i < 256; i++)
        {
            Palette[i].rgbRed   = (BYTE)i;
            Palette[i].rgbGreen = (BYTE)i;
            Palette[i].rgbBlue  = (BYTE)i;
        }

        // write image as a tga
        TgaWrite(dlg.GetPathName(), &Elevation);
    }
}
