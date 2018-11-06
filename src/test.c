#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

int main( int argc, char *argv[] )
{
	int n;
	char *option, *value;
	FILE *src, *dst;

	src = stdin;
	dst = stdout;

	for( n = 1; n < argc; n = n + 2 )
	{
		option = argv[n];
		value = argv[n + 1];

		switch ( (int) option[1] )
		{
			case 'i':
			case 'I':
				if ( ( src = fopen( value, "r" )) == NULL )
				{
					puts( "Can't open input file.\n" );
					exit( 0 );
				}

				break;

			case 'o':
			case 'O':
				if ( ( dst = fopen( value, "w" )) == NULL )
				{
					puts( "Can't open output file.\n" );
					exit( 0 );
				}
				break;

			default:
				printf( "Unknown option %s\n", option );
				exit( 0 );
		}
	}

	char b[MAX];

	/* Copy one file to the next. */

	while( ( fgets( b, MAX, src ) ) != NULL )
	{
		fputs( b, dst );
	}

	/* All done, close up shop. */

	fclose( src );
	fclose( dst );
}