//
//  ===========================================================================
//  LOS TABLES
//

/*

00000: 03c00000 00000000 00000000 00000000  .@..............
00010: 000003c0 00000000 00000000 00000000  ...@............
00020: 00000000 03c00000 00000000 00000000  .....@..........
00030: 00000000 000003c0 00000000 00000000  .......@........
00040: 00000000 00000000 07c00100 00000000  .........@......
00050: 00000000 00000000 000007c0 01000000  ...........@....
00060: 00000000 00000000 00000000 07c00100  .............@..
00070: 00000000 00000000 00000000 000007c0  ...............@


Take number of tiles (wxh and divide by 3 to get mega/meta tiles - size of deployment zones)

Example 
map with 36x36 tiles
has 12x12 mega tiles (or a 144 mega tiles)

144/8 = 18 bytes per row (round result up) 

File is made up of 144 4x18 byte rows

each row is a bitfield (0 can't see, 1 can see) from that mega tile to every other mega tile
the 4 rows correspond to 
soldier viewing soldier
soldier viewing vehicle
vehicle viewing soldier
vehicle viewing vehicle

vehicle is used for standing soldier

los files are only used for strategic ai and therefore have no effect on 2 player games
a file with all zeros if valid but the AI wouldn't play worth a shit

*/

typedef struct
{
    PBYTE pSxS;               // Pointer to Soldier to Soldier LOS bits
    PBYTE pSxV;               // Pointer to Soldier to Vehicle LOS bits
    PBYTE pVxS;               // Pointer to Vehicle to Soldier LOS bits
    PBYTE pVxV;               // Pointer to Vehicle to Vehicle LOS bits
} LOSTILEDATA;

typedef struct
{
    DWORD X;                  // Tile width of map
    DWORD Y;                  // Tile height of map
    DWORD BlockSize;          // Length of LOS blocks
    PBYTE pBuffer;            // LOS Buffer
    LOSTILEDATA *Tile;       // LOS data for each tile
} LINEOFSIGHTDATA,*PLINEOFSIGHTDATA;





