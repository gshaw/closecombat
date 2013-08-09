#ifndef __MAPDOC_H
#define __MAPDOC_H

////////////////////////////////////////////////////////////////////////////////
// CMapDoc

class CMapDoc : public CDocument
{
// Construction/Destruction
public:
	CMapDoc();
	virtual ~CMapDoc();
	DECLARE_DYNCREATE(CMapDoc)

// Attributes
public:
	int GetWidth() const;
	int GetHeight() const;

	CTile GetTile(int Row) const;
	void SetTile(int Row, int E, int Type);
	void SetTileElevation(int Row, int Elevation);

	CDIB*      m_pMapImage; // bitmap that makes up map

// Operations
public:
	void Resize(int Width, int Height);	// pixels (must be in multiplies of 120, ie 120,240,360, etc..)

// overrides
	//{{AFX_VIRTUAL(CMapDoc)
	public:
	virtual void DeleteContents();
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR pPathName);
	virtual BOOL OnSaveDocument(LPCTSTR pPathName);
	//}}AFX_VIRTUAL

// implementation
protected:
	BOOL ReadMapTiles(const CString& PathName);	// return TRUE if everything is ok
	BOOL ReadMapImage(const CString& PathName);	// return TRUE if everything is ok

	BOOL WriteMapTiles(const CString& PathName);	// return TRUE if everything is ok

	int	m_Width;		// dimensions of map in mega-tiles
	int	m_Height;

	CTileArray m_MapTiles;	// w*h tiles make up a map


// message map
protected:
	//{{AFX_MSG(CMapDoc)
	afx_msg void OnFileOpen();
	afx_msg void OnImportMap();
	afx_msg void OnExportMap();
	afx_msg void OnExportElevation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif //__MAPDOC_H
