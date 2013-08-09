#include "StdAfx.h"
#include "DIB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIB creation

CDIB::CDIB(bool IsFlipped)
{
    m_pBMI    = NULL;
    m_pBase   = NULL;
    m_hBitmap = NULL;

    m_IsFlipped = IsFlipped;
}

CDIB::~CDIB()
{
    if (m_pBMI != NULL)
    {
        delete m_pBMI;
    }

    if (m_hBitmap != NULL)
    {
        ASSERT(m_pBase != NULL);
        DeleteObject(m_hBitmap);    // deleting the handle will free m_pBase data
    }

    m_pBMI = NULL;
    m_pBase   = NULL;
    m_hBitmap = NULL;
}


void CDIB::Create(int width, int height, int bpp)
{
    ASSERT(width  > 0 && width  < 3000);    // checks for probable garbage upper limit (3000) increase if neccessary
    ASSERT(height > 0 && height < 3000);
    ASSERT(bpp == 1 || bpp == 4 || bpp == 8 || bpp == 16 || bpp == 24 || bpp == 32);

    // caller must call destroy before creating a new dib
    ASSERT(m_pBMI == NULL && m_pBase == NULL && m_hBitmap == NULL);

    // allocate header
    m_pBMI = (BITMAPINFO*) new BYTE[GetBitmapInfoSize(bpp)];

    // fill in header
    BITMAPINFOHEADER* pBI   = (BITMAPINFOHEADER*)m_pBMI;
    pBI->biSize             = sizeof(BITMAPINFOHEADER);
    pBI->biWidth            = width;
    pBI->biHeight           = (m_IsFlipped == FALSE) ? -height : height;
    pBI->biPlanes           = 1;
    pBI->biBitCount         = (WORD)bpp;
    pBI->biCompression      = GetCompressionMethod(bpp);

    pBI->biSizeImage        = 0;
    pBI->biXPelsPerMeter    = 0;
    pBI->biYPelsPerMeter    = 0;
    pBI->biClrImportant     = 0;
    pBI->biClrUsed          = 0;    // will be set when bmiColors are set if need be
                                    // zero means bmp will use max number it can (default)

    // fill in bmiColors field
    if (bpp == 1)
    {
        RGBQUAD* pRGB = GetColorTable();
        pRGB[0].rgbBlue = pRGB[0].rgbGreen = pRGB[0].rgbRed = 0;
        pRGB[1].rgbBlue = pRGB[1].rgbGreen = pRGB[1].rgbRed = 255;
        pRGB[0].rgbReserved = pRGB[1].rgbReserved = 255;
    }
    if (bpp == 4 || bpp == 8)
    {
        RGBQUAD* pRGB = GetColorTable();
        int num = (bpp==4) ? 16 : 256;
        for (int i = 0; i < num; i++)
        {
            pRGB->rgbBlue  = (unsigned char)(i*256/(num));
            pRGB->rgbGreen = (unsigned char)(i*256/(num));
            pRGB->rgbRed   = (unsigned char)(i*256/(num));
            pRGB->rgbReserved = 255;
            pRGB++;
        }
    }
    else if (bpp == 16) // 555
    {
        DWORD* mask = (DWORD*)GetColorTable();
        mask[0] = 0x7c00;       // red   (01111100 00000000)
        mask[1] = 0x03e0;       // green (00000011 11100000)
        mask[2] = 0x001f;       // blue  (00000000 00011111)
        pBI->biSizeImage = static_cast<DWORD>(GetHeight() * GetRowBytes());
    }
    else if (bpp == 32)
    {
        DWORD* mask = (DWORD*)GetColorTable();
        mask[0] = 0x00ff0000;   // red   (00000000 11111111 00000000 00000000)
        mask[1] = 0x0000ff00;   // green (00000000 00000000 11111111 00000000)
        mask[2] = 0x000000ff;   // blue  (00000000 00000000 00000000 11111111)
        pBI->biClrUsed = 3;
        pBI->biSizeImage = static_cast<DWORD>(GetHeight() * GetRowBytes());
    }

    // allocate frame buffer
    m_hBitmap = CreateDIBSection(NULL, m_pBMI, DIB_RGB_COLORS, (void**)(&m_pBase), NULL, 0);
    if (m_hBitmap == NULL || m_pBase == NULL)
    {
        TRACE1("CreateDIBSection failed with error code: %d\n", GetLastError());
        ASSERT(FALSE);
        AfxThrowMemoryException();
    }
}

////////////////////////////////////////////////////////////////////////////////

DWORD  CDIB::GetColorTableSize(int bpp)
{
    switch (bpp)
    {
        case  1: return   2*sizeof(RGBQUAD);
        case  4: return  16*sizeof(RGBQUAD);
        case  8: return 256*sizeof(RGBQUAD);
        case 16: return   3*sizeof(DWORD);
        case 24: return   0;
        case 32: return   3*sizeof(DWORD);

        default:
            ASSERT(FALSE);
            return 0;
    }
}

DWORD CDIB::GetBitmapInfoSize(int bpp)
{
    return sizeof(BITMAPINFOHEADER) + GetColorTableSize(bpp);
}

DWORD  CDIB::GetCompressionMethod(int bpp)
{
    switch (bpp)
    {
        case  1: return BI_RGB;
        case  4: return BI_RGB;
        case  8: return BI_RGB;
        case 16: return BI_BITFIELDS;
        case 24: return BI_RGB;
        case 32: return BI_BITFIELDS;

        default:
            ASSERT(FALSE);
            return 0;
    }
}


void CDIB::Flip()
{
    ASSERT_VALID(this);

    // flips the internal representation of the bits in the bitmap
    // call before coping to the clipboard

    // invert height field
    m_pBMI->bmiHeader.biHeight = -(m_pBMI->bmiHeader.biHeight);

    // create temp row buffer on stack to prevent failing?
    int RowBytes  = GetRowBytes();
    BYTE* pBuffer = new BYTE[static_cast<DWORD>(RowBytes)];

    // set up top and bottom ptrs
    BYTE* pTop    = GetBase();
    BYTE* pBottom = GetBase() + RowBytes*(GetHeight() - 1);

    // swap top and bottom rows for half the height of bitmap
    for (int row = 0; row < GetHeight()/2; row++)
    {
        CopyMemory(pBuffer, pTop,    static_cast<DWORD>(RowBytes));
        CopyMemory(pTop,    pBottom, static_cast<DWORD>(RowBytes));
        CopyMemory(pBottom, pBuffer, static_cast<DWORD>(RowBytes));

        pTop    += RowBytes;
        pBottom -= RowBytes;
    }

    delete pBuffer;
}


/////////////////////////////////////////////////////////////////////////////
// Private functions

DWORD CDIB::NumDIBColorEntries(BITMAPINFO* pBmpInfo)
{
    ASSERT(pBmpInfo);
    BITMAPINFOHEADER* pBIH = &(pBmpInfo->bmiHeader);

    // start off by assuming the color table size from the bit per pixel field
    int Bpp = pBIH->biBitCount;

    DWORD NumColors;
    switch (Bpp)
    {
        case 1:
            NumColors = 2;
            break;
        case 4:
            NumColors = 16;
            break;
        case 8:
            NumColors = 256;
            break;
        default:
            NumColors = 0;
            break;
    }

    if (Bpp > 8)
        return 0;

    // If this is a Windows DIB, then the color table length
    // is determined by the biClrUsed field if it is non-zero
    if (pBIH->biClrUsed != 0)
        NumColors = pBIH->biClrUsed;

    return NumColors;
}

/////////////////////////////////////////////////////////////////////////////
// CDIB accessors

BITMAPINFO* CDIB::GetBitmapInfo()
{
    return m_pBMI;
}

HBITMAP CDIB::GetBitmap()
{
    return m_hBitmap;
}

BYTE* CDIB::GetBase()
{
    return m_pBase;
}

int CDIB::GetWidth() const
{
    return m_pBMI->bmiHeader.biWidth;
}

int CDIB::GetHeight() const
{
    return abs(m_pBMI->bmiHeader.biHeight);
}

int CDIB::GetRowBytes() const
{
    // DIB's are 4 byte aligned
    int Bpp   = GetBpp();
    int Width = GetWidth();

    return (Bpp * Width + 31)/32 * 4;
}

int CDIB::GetBpp() const
{
    return m_pBMI->bmiHeader.biBitCount;
}


/////////////////////////////////////////////////////////////////////////////
// Palette handling

RGBQUAD* CDIB::GetColorTable()
{
    return (RGBQUAD*)((LPSTR)m_pBMI + (WORD)(m_pBMI->bmiHeader.biSize));
}

int CDIB::GetColorTableCount()
{
    // get the number of color table entries
    return static_cast<int>(NumDIBColorEntries(m_pBMI));
}


/////////////////////////////////////////////////////////////////////////////
// Draw handling

// optimized for drawing into a scroll view
void CDIB::Draw(CDC* pDC, const CRect& D, const CRect& S)
{
    ::StretchDIBits(pDC->GetSafeHdc(),
        D.left,     D.top,      // dst offset
        D.Width(),  D.Height(), // dst size
        S.left,     S.top,      // src offset
        S.Width(),  S.Height(), // src size
        GetBase(),              // bits
        GetBitmapInfo(),        // BITMAPINFO
        DIB_RGB_COLORS,         // options
        SRCCOPY);               // rop
}


/////////////////////////////////////////////////////////////////////////////
// CDIB diagnostics

#ifdef _DEBUG
void CDIB::AssertValid() const
{
    ASSERT(AfxIsValidAddress(m_pBMI,  sizeof(BITMAPINFOHEADER)));
    ASSERT(AfxIsValidAddress(m_pBase, GetRowBytes()*GetHeight()));
    ASSERT(m_hBitmap != NULL);

    int bpp = GetBpp();
    int w = GetWidth();
    int h = GetHeight();

    ASSERT(w > 0 && w < 3000);  // checks for probable garbage upper limit (3000) increase if neccessary
    ASSERT(h > 0 && h < 3000);
    ASSERT(bpp == 1 || bpp == 4 || bpp == 8 || bpp == 16 || bpp == 24 || bpp == 32);

    CObject::AssertValid();
}

void CDIB::Dump(CDumpContext& dc) const
{
    CObject::Dump(dc);
    dc << "Width = "  << GetWidth()  << " ";
    dc << "Height = " << GetHeight() << " ";
    dc << "Bpp = "    << GetBpp()    << " ";
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDIB streaming (for clipboard support)

void CDIB::Write(CFile* pFile)
{
    // write BITMAPINFO header and data
    int size = GetBitmapInfoSize(GetBpp());
    BITMAPINFO* pBMI = GetBitmapInfo();
    pFile->Write(pBMI, size);
    pFile->Write(GetBase(), GetRowBytes()*GetHeight());
}

void CDIB::Read(CFile* pFile)
{
    // get header size
    DWORD bmiSize;
    pFile->Read(&bmiSize, sizeof(DWORD));
    pFile->Seek(0 - sizeof(DWORD), CFile::current); // return file pos to start of file

    // get bitmap header
    BITMAPINFO bmi;     // has BITMAPINFOHEADER
    pFile->Read(&bmi, bmiSize);

    // create a new pImage based on header
    int width  = bmi.bmiHeader.biWidth;
    int height = abs(bmi.bmiHeader.biHeight);
    int bpp    = bmi.bmiHeader.biBitCount;

    CDIB* pDIB = new CDIB(true);
    pDIB->Create(width,height,bpp);

    // read color table
    int ColorTblSize = CDIB::GetColorTableSize(bpp);
    RGBQUAD* pColorTbl = pDIB->GetColorTable();
    pFile->Read(pColorTbl, ColorTblSize);

    // read frame buffer
    int rowbytes = pDIB->GetRowBytes();
    BYTE* pBase  = pDIB->GetBase();
    ASSERT(pBase != NULL);
    FillMemory(pBase, height*rowbytes, 0xff);
    pFile->Read(pBase, rowbytes * height);

    //////////////////////////////////////////////////////
    // now copy dib frame buffer into 8/32 image buffer

    // create frame buffer to hold data for this pImage
    Create(width, height, (bpp <= 8) ? 8 : 32);

    // create dst DC
    CDC dstDC;
    dstDC.CreateCompatibleDC(NULL);

    // select frame buffer into dst DC
    CBitmap* pDstBmp = CBitmap::FromHandle(m_hBitmap);
    dstDC.SelectObject(pDstBmp);

    // create src DC
    CDC srcDC;
    srcDC.CreateCompatibleDC(NULL);

    // select pDIB into src DC
    CBitmap* pSrcBmp = CBitmap::FromHandle(pDIB->m_hBitmap);
    srcDC.SelectObject(pSrcBmp);

    // copy src DC into dst DC
    dstDC.BitBlt(0,0,width,height, &srcDC, 0,0, SRCCOPY);

    // now if bpp == 24 we need to set our alpha channel to opaque since the
    // clipboard does not set the data resulting in an undefined clipboard
    if (bpp > 8)
    {
        // check to see if entire alpha channel is 0, if so then make it opaque
        // otherwise leave it alone

        int w  = GetWidth();
        int h  = GetHeight();
        int rb = GetRowBytes();

        bool Transparent = true;

        // search for the first non-zero value
        for (int row = 0; row < h; row++)
        {
            unsigned long* dst = (unsigned long*)(m_pBase + row*rb);
            for (int col = 0; col < w; col++)
            {
                if (((*dst) & 0xff000000) != 0)
                {
                    Transparent = false;
                    break;
                }
                dst++;
            }
            if (!Transparent)
                break;
        }

        if (Transparent)
        {
            for (int row = 0; row < h; row++)
            {
                unsigned long* dst = (unsigned long*)(m_pBase + row*rb);
                for (int col = 0; col < w; col++)
                    *dst++ |= 0xff000000;
            }
        }
    }
    else
    {
        // copy palette
        CopyMemory(GetColorTable(), pDIB->GetColorTable(), GetColorTableSize(bpp));

        int NumColors = GetColorTableCount();

        // check to see if entire palette is transparent, and if so make it opaque
        int OpaqueCount = 0;

        RGBQUAD* rgbq = GetColorTable();
        for (int i = 0; i < NumColors; i++)
        {
            if (rgbq[i].rgbReserved == 255)
                OpaqueCount++;
        }

        // if it seems *likely* that the palette is transparent then make it opaque
        // this check is done by seeing if the number of nontransparent colors is near zero
        if (OpaqueCount <= 0)
        {
            for (i = 0; i < NumColors; i++)
            {
                rgbq[i].rgbReserved = 255;
            }
        }

    }

    // free used memory
    delete pDIB;
}

