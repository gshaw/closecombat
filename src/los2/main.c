
#include	<stdio.h>
#include	<string.h>
#include	"fcclos.h"

void add_backslash ( char * path )
{
	int i ;

	for ( i = 0 ; path[i] != 0 ; i++ ) ;
	i-- ;

	if (path[i] != '\\')
	{
		path[i+1] = '\\' ;
		path[i+2] = 0 ;
	}
}

int main( int argc, char *argv[ ], char *envp[ ] )
{
	char temp[5] ;

	char path_elements[200] = "data\\base\\elements" ;

	char path_map[200] = "data\\maps\\map" ;
	int path_map_def = 0;

	char path_los[200] = "data\\maps\\map" ;
	int path_los_def = 0;

	long i=0, mapnum=0 ;
	int j;

	CClos_TextOutput = 1 ;

	if ( argc > 1 )
	{
		for ( j = 1 ; j < argc ; j++ )
		{
			if (argv[j][0] == '+' || argv[j][0] == '-')
			{
				switch (argv[j][1])
				{
				case 'T':
					CClos_TextOutput = 0 ;
					break;
				case 'M':
					strcpy ( path_map , argv[j+1] ) ;
					add_backslash ( path_map ) ;
					strcat ( path_map , "map" ) ;
					path_map_def = 1 ;
					if ( !path_los_def)
						strcpy ( path_los , path_map ) ;
					break ;
				case 'L':
					strcpy ( path_los , argv[j+1] ) ;
					add_backslash ( path_los ) ;
					strcat ( path_los , "map" ) ;
					path_los_def = 1 ;
					if ( !path_map_def)
						strcpy ( path_map , path_los ) ;
					break ;
				case 'E':
					strcpy ( path_elements , argv[j+1] ) ;
					break ;
				case 'N':
					i = sscanf ( argv[j+1] , "%d" , &mapnum ) ;
					break ;
				}
			}
		}
	}
	if (CClos_TextOutput)
		printf ("CC2 LOS calculation - v0.99b\n");

	while ( i == 0 || mapnum > 999 )
	{
		if (!CClos_TextOutput)
			return -8;

		printf ("enter map number : ");
		i = scanf ( "%d" , &mapnum ) ;

		if ( i == 0 )
			printf ("only the number...\n");

		if ( mapnum > 999 )
			printf ("number too hight...\n");

		if ( i != 0 && mapnum < 999 )
			printf ("\n");
	}

	sprintf ( temp , "%03d" , mapnum ) ;

	strcat ( path_map , temp ) ;
	strcat ( path_los , temp ) ;
	strcat ( path_los , ".los" ) ;

	fflush ( stdin ) ;
	i = LOScalculate ( path_map, path_los, path_elements ) ;

	if (CClos_TextOutput)
		switch (i)
		{
		case 0:
			printf ( "All OK!\n" ) ;
			getchar ();
			break ;
		case -1:
			printf ( "ERROR : bad map file\n" ) ;
			getchar ();
			break ;
		case -2:
			printf ( "ERROR : bad elements file\n" ) ;
			getchar ();
			break ;
		case -3:
			printf ( "ERROR : not enough memory\n" ) ;
			getchar ();
			break ;
		case -4:
			printf ( "ERROR : map file not found\n" ) ;
			getchar ();
			break ;
		case -5:
			printf ( "ERROR : element file not found\n" ) ;
			getchar ();
			break ;
		case -6:
			printf ( "ERROR : can't create LOS file\n" ) ;
			getchar ();
			break ;
		case -7:
			printf ( "ERROR : can't write in LOS file (disk full)\n" ) ;
			getchar ();
			break ;
		case -8:
			printf ( "ERROR : bad element in map\n" ) ;
			getchar ();
			break ;
		}

	if (CClos_TextOutput)
			return 0 ;
	else	return i ;
}
