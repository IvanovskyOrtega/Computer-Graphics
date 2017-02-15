#include<stdio.h>
#include<stdlib.h>
#include"functions.h"

void lineProgram()
{	
	int **Raster=NULL;
	double x0 = 0;
	double y0 = 0;
	double x1 = 0;
	double y1 = 0;
	double slope = 0;
	double b = 0;
	int width = 0;
	int height = 0;
	printf("Welcome to the DrawLine Program :D\n");
	printf("Please insert the raster dimensions to start...");
	printf("\nWidth:	");
	scanf("%d",&width);
	printf("Heigth:	");
	scanf("%d",&height);
	Raster = (int **)malloc(width*sizeof(int *));
	for (int i = 0 ; i < width ; i++)
	{
		Raster[i] = (int *) malloc (height*sizeof(int));
	}
	for(int i = 0 ; i < width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			Raster[i][j]=0;
		}
	}
	printf("\nType the points where the line passes by.\nFormat (x0,y0),(x1,y1)");
	printf("\nx0:	");
	scanf("%lf",&x0);
	printf("y0:	");
	scanf("%lf",&y0);
	printf("x1:	");
	scanf("%lf",&x1);
	printf("y1:	");	
	scanf("%lf",&y1);
	slope = ( y1 - y0 )/( x1 - x0 );
	b = y0 - (slope*x0);
	printf("The line ecuation is: y = %lfx + %lf\n",slope,b);
	fillRaster(x0,y0,x1,y1,slope,b,Raster);
	drawLine(Raster,width,height);
}

void fillRaster( double x0, double y0, double x1, double y1, double slope, double b, int **Raster )
{
	putPixel( x0 , y0 , Raster );
	for( int i = x0 + 1 ; i <= x1 ; i++ )
	{
		y0 = y0 + slope;
		putPixel( i , (int)y0 , Raster );
	}
}

void putPixel( int x, int y, int **Raster )
{
	Raster[x][y]=1;
}
void drawLine( int **Raster, int width, int height )
{
	for( int i = height - 1 ; i >=0 ; i-- )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			if(Raster[ j ][ i ]==0){
				printf("| |");			
			}
			else
			{
				printf("|*|");
			}
		}
		printf("\n");
	}
}
