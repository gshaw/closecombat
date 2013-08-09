// Mapxxx structures
// from ccUtils.h

//
//  ===========================================================================
//	MAP - ELEMNT XREF, AND ELEVATIONS
//  
//  Standard text file, version ID, then data begins after '&' and end with '#'

#define FILE_MAPXREFDATA_ELEMENTSPERTILE	16

typedef struct
{
	//	IGNORE INDEX WORD

	DWORD	dwElement[FILE_MAPXREFDATA_ELEMENTSPERTILE];					//	
	DWORD	dwElevation;																					//	
} MAPINDEXDATA, *PMAPINDEXDATA;

typedef struct
{
	DWORD					dwX;																					//	
	DWORD					dwY;																					//	
	MAPINDEXDATA			*Index;																			//	
} MAPXFERDATA, *PMAPXFERDATA;

#define FILE_MAPXREFDATA_VERSIONTAG			41
