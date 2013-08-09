// LOScalculate
//	mapfile : map name and path
//  losfile : los name and path
//	element : element file used (path and name)
//	return : 0 - all Ok
//			-1 - bad map file
//			-2 - bad element file
//			-3 - not enought memory
//			-4 - map file not found
//			-5 - element file not found
//			-6 - can't create los file (write protect)
//			-7 - disk full
//			-8 - bad element in map

extern int	CClos_TextOutput ;

int LOScalculate ( char* mapfile, char* losfile, char* element ) ;
