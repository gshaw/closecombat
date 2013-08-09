#ifndef __DATATYPES_H
#define __DATATYPES_H

////////////////////////////////////////////////////////////////////////////////
// CElement, CElementArray

class CElement
{
public:
	CString  Name;
	int		 Type;					
	int		 Height;				// 1/2 meters
	int		 TileType;				// 0-30 (see elements data file for explanation)
	COLORREF Color;

	CElement()
	{
		Type = 0; Height = 0; TileType = 0; Color = RGB(0,0,0);
	};
};

typedef CArray<CElement, CElement&> CElementArray;

////////////////////////////////////////////////////////////////////////////////
// CTile, CTileArray

class CTile
{
public:
	int Row;
	int	E[16];
	int Elevation;

	CTile()
	{
		Row = 0;
		Elevation = 0;
		ZeroMemory(E, sizeof(E));
	};
};

typedef CArray<CTile, CTile&> CTileArray;

////////////////////////////////////////////////////////////////////////////////
// Texture and Map Image file header

typedef struct 
{
	unsigned char header[4];
	unsigned char flags[4];		// 0x00020000
	unsigned char width[4];
	unsigned char height[4];
} TXTR_HEADER;

typedef TXTR_HEADER	MAP_HEADER;

////////////////////////////////////////////////////////////////////////////////
// Element types

/*
0 = Open
1 = Concealing
2 = Gulley
3 = Trees
4 = Bocage
5 = Road
6 = Bridge
7 = Fortification
8 = Bunker
9 = Wall
10 = Marsh
11 = Wire
12 = Water
13 = Wood Bld
14 = Brick Bld
15 = Stone Bld
16 = Deep Water
17 = Bld Interior
18 = Lt Wall
19 = Hvy Wall
20 = Improved Pos
21 = Wood Bld L2
22 = Wood Bld L3
23 = Brick Bld L2
24 = Brick Bld L3
25 = Stone Bld L2
26 = Stone Bld L3
27 = Stone Bld L4
28 = Interior L2
29 = Interior L3
30 = Interior L4
*/

extern COLORREF g_ElementTypeColors[];

#endif //__DATATYPES_H
