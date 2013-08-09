// Element file datas (data/base/elements)
// from ccUtils.h

typedef int CCVAL;
typedef int CCFLG;
//
//  ===========================================================================
//	ELEMENTS
//  
//  Standard text file, version ID, then data begins after '&' and end with '#'

typedef struct
{
  char    szName[50];                                         //  
  DWORD	  dwIndex;
  DWORD   dwHeight;                                           //  in .5 meter increments
  CCVAL   dwCover_Prone;                                      //  (0 - 512)
  CCVAL   dwCover_Low;                                        //  (0 - 512)
  CCVAL   dwCover_Medium;                                     //  (0 - 512)
  CCVAL   dwCover_High;                                       //  (0 - 512)
  DWORD   dwProtection_Prone;                                 //  mm of steel
  DWORD   dwProtection_Low;                                   //  mm of steel
  DWORD   dwProtection_Medium;                                //  mm of steel
  DWORD   dwProtection_High;                                  //  mm of steel
  CCVAL   dwHindrance_Prone;                                  //  (0 - 512)
  CCVAL   dwHindrance_Low;                                    //  (0 - 512)
  CCVAL   dwHindrance_Medium;                                 //  (0 - 512)
  CCVAL   dwHindrance_High;                                   //  (0 - 512)
  CCFLG   bBlockHeight;																				//  
  CCFLG   bBlockAll;																					//  
  DWORD   dwTopProtection;                                    //  mm of steel
  DWORD   dwInElementProtectionFlag;                          //  
  DWORD   dwHEEffect;                                         //  divisor
  DWORD   dwSoldierMoveRate_Prone;                            //  secs/2 meters
  DWORD   dwSoldierMoveRate_Crouch;                           //  secs/2 meters
  DWORD   dwSoldierMoveRate_Standing;                         //  secs/2 meters
  DWORD   dwVehicleMoveRate_Truck;                            //  secs/2 meters
  DWORD   dwVehicleMoveRate_ArmoredCar;                       //  secs/2 meters
  DWORD   dwVehicleMoveRate_Halftrack;                        //  secs/2 meters
  DWORD   dwVehicleMoveRate_Tracked;                          //  secs/2 meters
  DWORD   dwVehicleMoveRate_Pushed;                           //  secs/2 meters
  DWORD   dwBogChance;                                        //  % of 128
  CCFLG   bFlag_Wall;																					//  
  CCFLG   bFlag_Building;																			//  
  CCFLG   bFlag_Sunken;																				//  
  CCFLG   bFlag_Crushable;																		//  
  CCFLG   bFlag_Covered;																			//  
  CCFLG   bFlag_Bridge;																				//  
  CCFLG   bFlag_Flammable;																		//  
  DWORD   dwBridgeDirection;                                  //  
  DWORD   dwExplode;                                          //  CC_ELEMENTDATA_EXPLODETYPE_*
  DWORD   dwTileType;                                         //  CC_ELEMENTDATA_TILETYPE_*
  DWORD   dwLevel;                                            //  (0 - 3)
  DWORD   dwRubbleTo;                                         //  Element ID to transform to
  DWORD   dwCrushTo;                                          //  Element ID to transform to
} ELEMENTDATA, *PELEMENTDATA;

#define FILE_ELEMENTDATA_VERSIONTAG									46

#define CC_ELEMENTDATA_TILETYPE_OPEN                0
#define CC_ELEMENTDATA_TILETYPE_CONCEALING          1
#define CC_ELEMENTDATA_TILETYPE_GULLEY              2
#define CC_ELEMENTDATA_TILETYPE_TREES               3
#define CC_ELEMENTDATA_TILETYPE_BOCAGE              4
#define CC_ELEMENTDATA_TILETYPE_ROAD                5
#define CC_ELEMENTDATA_TILETYPE_BRIDGE              6
#define CC_ELEMENTDATA_TILETYPE_FORTIFICATION       7
#define CC_ELEMENTDATA_TILETYPE_BUNKER              8
#define CC_ELEMENTDATA_TILETYPE_WALL                9
#define CC_ELEMENTDATA_TILETYPE_MARSH               10
#define CC_ELEMENTDATA_TILETYPE_WIRE                11
#define CC_ELEMENTDATA_TILETYPE_WATER               12
#define CC_ELEMENTDATA_TILETYPE_WOODBUILDING        13
#define CC_ELEMENTDATA_TILETYPE_BRICKBUILDING       14
#define CC_ELEMENTDATA_TILETYPE_STONEBUILDING       15
#define CC_ELEMENTDATA_TILETYPE_DEEPWATER           16
#define CC_ELEMENTDATA_TILETYPE_BUILDINGINTERIOR    17
#define CC_ELEMENTDATA_TILETYPE_LIGHTWALL           18
#define CC_ELEMENTDATA_TILETYPE_HEAVYWALL           19
#define CC_ELEMENTDATA_TILETYPE_IMPROVEDPOSITION    20
#define CC_ELEMENTDATA_TILETYPE_WOODBUILDING_L2     21
#define CC_ELEMENTDATA_TILETYPE_WOODBUILDING_L3     22
#define CC_ELEMENTDATA_TILETYPE_BRICKBUILDING_L2    23
#define CC_ELEMENTDATA_TILETYPE_BRICKBUILDING_L3    24
#define CC_ELEMENTDATA_TILETYPE_STONEBUILDING_L2    25
#define CC_ELEMENTDATA_TILETYPE_STONEBUILDING_L3    26
#define CC_ELEMENTDATA_TILETYPE_STONEBUILDING_L4    27
#define CC_ELEMENTDATA_TILETYPE_BUILDINGINTERIOR_L2 28
#define CC_ELEMENTDATA_TILETYPE_BUILDINGINTERIOR_L3 29
#define CC_ELEMENTDATA_TILETYPE_BUILDINGINTERIOR_L4 30

#define CC_ELEMENTDATA_EXPLODETYPE_NONE             0
#define CC_ELEMENTDATA_EXPLODETYPE_CRATER           1
#define CC_ELEMENTDATA_EXPLODETYPE_SPLASH           2
#define CC_ELEMENTDATA_EXPLODETYPE_WOOD             3
#define CC_ELEMENTDATA_EXPLODETYPE_STONE            4
#define CC_ELEMENTDATA_EXPLODETYPE_BRICK            5

#define CC_ELEMENTDATA_MAX                          145
