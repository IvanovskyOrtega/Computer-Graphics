#include <stdio.h>
#include "functions.h"

void generateFlag()
{
	int i=0;
	int j=0;
	printf("P3\n");
	printf("1920 1080\n");
	printf("255\n");
	for( i = 0 ; i < 1080 ; i++ )
	{
		for( j = 0 ; j < 1920 ; j++ )
		{
			if( j < 640 )
				printf(" 0 255 0 ");
			else if (j >= 640 && j < 1280 && i < 360)
				printf(" 255 255 255 ");
			else if (j >= 640 && j < 1280 && i >= 720)
				printf(" 255 255 255 ");
			else if (j >= 640 && j < 1280 && i >= 360 && i < 720 )
				printf(" 156 102  31 ");
			else if( j >= 1280 )
				printf(" 255 0 0 ");
		}
		printf("\n");
	}	
}

