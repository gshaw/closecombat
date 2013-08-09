#ifndef __DIB_H
#define __DIB_H

class CDIB : public CObject
{
// Construction
public:
    CDIB(bool IsFlipped = false);
    virtual ~CDIB();

    // create a new DIB (calls destroy)
	virtual void Create(int width, int height, int bpp);

	// call to invert representation of DIB (ie, before coping to clipboard)
	virtual void Flip();

	// streaming support so that we can copy/paste from CClipboard
	virtual void Read(CFile* pFile);
	virtual void Write(CFile* pFile);

	// accessors
    BITMAPINFO* GetBitmapInfo();				// ptr to bitmap info
    HBITMAP     GetBitmap();					// ptr to GDI bitmap object
    BYTE*       GetBase();						// ptr to the bits
    int 		GetWidth() const;				// image width in pixels
	int 		GetHeight() const;				// image height in pixels
	int 		GetRowBytes() const;			// number of bytes per row
	int 		GetBpp() const;					// number bits per pixel (8 or 32)

	// palette handling
    RGBQUAD*    GetColorTable();				// ptr to color table
    int     	GetColorTableCount();			// number of color table entries
    
    // draw handling
	virtual void Draw(CDC *pDC, const CRect& DstRect, const CRect& SrcRect);

	// diagnostic handling
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// implementation
private:
    BITMAPINFO* m_pBMI;         // pointer to BITMAPINFO struct
    BYTE*       m_pBase;        // pointer to frame buffer
	HBITMAP		m_hBitmap;		// GDI bitmap object that can be selected
	bool		m_IsFlipped;	// true if image is flipped

	static DWORD NumDIBColorEntries(BITMAPINFO* pBmpInfo);
	static DWORD GetColorTableSize(int bpp);
	static DWORD GetBitmapInfoSize(int bpp);
	static DWORD GetCompressionMethod(int bpp);

};

/////////////////////////////////////////////////////////////////////////////

#endif // __DIB_H
