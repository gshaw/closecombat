#include "stdafx.h"

////////////////////////////////////////////////////////////////////////////////
// Element types

COLORREF g_ElementTypeColors[] =
{
    RGB(128,255,128),       //  0 = Open
    RGB(  0,128,  0),       //  1 = Concealing
    RGB( 64,192, 64),       //  2 = Gulley
    RGB( 64,255, 64),       //  3 = Trees
    RGB( 92,255, 64),       //  4 = Bocage
    RGB( 92, 92, 92),       //  5 = Road
    RGB( 92, 92, 92),       //  6 = Bridge
    RGB( 64, 64, 64),       //  7 = Fortification
    RGB( 64, 64, 64),       //  8 = Bunker
    RGB( 64, 64, 64),       //  9 = Wall
    RGB( 48, 48, 48),       // 10 = Marsh
    RGB(  0,  0,  0),       // 11 = Wire
    RGB( 64,128,228),       // 12 = Water
    RGB(128, 64,  0),       // 13 = Wood Bld
    RGB(128,  0,  0),       // 14 = Brick Bld
    RGB(114,141,141),       // 15 = Stone Bld
    RGB( 26, 12,152),       // 16 = Deep Water
    RGB(188,198,187),       // 17 = Bld Interior
    RGB(192,192,192),       // 18 = Lt Wall
    RGB(128,128,128),       // 19 = Hvy Wall
    RGB(150,128,128),       // 20 = Improved Pos
    RGB(128, 64,  0),       // 21 = Wood Bld L2
    RGB(128, 64,  0),       // 22 = Wood Bld L3
    RGB(128,  0,  0),       // 23 = Brick Bld L2
    RGB(128,  0,  0),       // 24 = Brick Bld L3
    RGB(114,141,141),       // 25 = Stone Bld L2
    RGB(114,141,141),       // 26 = Stone Bld L3
    RGB(114,141,141),       // 27 = Stone Bld L4
    RGB(255,209,164),       // 28 = Interior L2
    RGB(255,209,164),       // 29 = Interior L3
    RGB(255,209,164)        // 30 = Interior L4
};
