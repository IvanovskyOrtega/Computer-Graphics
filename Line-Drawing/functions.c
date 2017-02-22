#include<stdio.h>
#include<stdlib.h>
#include"functions.h"

void lineProgram()
{	
	int **Raster=NULL;
	double Points[4]={};
	int width = 0;
	int height = 0;
	double slope = 0;
	double b = 0;
	double aux;
	int op;
	int evaluatedSlope;
	printf("Welcome to the DrawLine Program :D\n");
	printf("Select the line drawing algorithm to use:\n");
	printf("(1) Naive\n(2) DDA\n(3) Bresenham\nType your choise:\t");
	scanf("%d",&op);
	printf("Please insert the raster dimensions to start...");
	printf("\nWidth:	");
	scanf("%d",&width);
	printf("Heigth:	");
	scanf("%d",&height);
	Raster = createRaster( Raster, width, height );
	Raster = emptyRaster( Raster, width, height );
	getXYValues( Points );
	if(Points[3] - Points[1] == 0)
	{	
		if(Points[2] < Points[0])
		{
			aux = Points[0];
			Points[0] = Points[2];
			Points[2] = aux;		
		}
		printf("\nSpecial case, the algorith isn't going to be used, but the the program will draw the line :D\n");
		printf("The line ecuation is: y = %lf\n",Points[3]);
		fillSpecialCase1( Points[0], Points[1], Points[2], Points[3], Raster );	
		drawLine( Raster, width, height, 0 );
	}
	else if(Points[2] - Points[0] == 0)
	{	
		if(Points[3] < Points[1])
		{
			aux = Points[1];
			Points[1] = Points[3];
			Points[3] = aux;		
		}
		printf("\nSpecial case, the algorith isn't going to be used, but the the program will draw the line :D\n");
		printf("The line ecuation is: x = %lf\n",Points[0]);
		fillSpecialCase2( Points[0], Points[1], Points[2], Points[3], Raster );	
		drawLine( Raster, width, height, 0 );
	}
	else
	{
		slope = ( Points[3] - Points[1] )/( Points[2] - Points[0] );
		b = Points[1] - (slope*(Points[0]));
		printf("The line ecuation is: y = %lfx + %lf\n",slope,b);
		switch(op)
		{
			case 1:
				evaluatedSlope = evaluateSlope( slope, Points );
				fillRasterNaive( Points[0], Points[1], Points[2], Points[3], slope, b, Raster, evaluatedSlope );
				drawLine( Raster, width, height, evaluatedSlope );
				break;
			case 2:
				evaluatedSlope = evaluateSlope( slope, Points );
				fillRasterDDA( Points[0], Points[1], Points[2], Points[3], slope, Raster, evaluatedSlope );
				drawLine( Raster, width, height, evaluatedSlope );
				break;
			case 3:
				evaluatedSlope = evaluateSlope( slope, Points );
				fillRasterBresenham( (int)Points[0], (int)Points[1], (int)Points[2], (int)Points[3], Raster, evaluatedSlope );
				drawLine( Raster, width, height, evaluatedSlope );
				break;
			default:
				printf("That's not a valid option, please try again :(\n");
				break;
		}
	}
	
	Raster = emptyRaster( Raster, width, height );
}

int** createRaster( int** Raster, int width, int height )
{
	Raster = (int **)malloc(width*sizeof(int *));
	for (int i = 0 ; i < width ; i++)
	{
		Raster[i] = (int *) malloc (height*sizeof(int));
	}
	return Raster;
}

int** emptyRaster( int** Raster, int width, int height  )
{
	for(int i = 0 ; i < width ; i++)
	{
		for(int j = 0 ; j < height ; j++)
		{
			Raster[i][j]=0;
		}
	}
	return Raster;
}

double* getXYValues( double* Points )
{
	double x0 = 0;
	double y0 = 0;
	double x1 = 0;
	double y1 = 0;
	printf("\nType the points where the line passes by.\nFormat (x0,y0),(x1,y1)");
	printf("\nx0:	");
	scanf("%lf",&x0);
	printf("y0:	");
	scanf("%lf",&y0);
	printf("x1:	");
	scanf("%lf",&x1);
	printf("y1:	");	
	scanf("%lf",&y1);
	if(x1 < x0)
	{
		Points[0] = x1;
		Points[1] = y1;
		Points[2] = x0;
		Points[3] = y0; 	
	}
	else
	{
		Points[0] = x0;
		Points[1] = y0;
		Points[2] = x1;
		Points[3] = y1; 
	}
	return Points;
}

int evaluateSlope( double slope, double* Points )
{
	if( slope > 1 )//The line is in the 2nd or 6th octect
	{
		return 2;
	}
	else if( slope < 0 && slope > -1 )//The line is in the 4th or 8th octect
	{
		return 3;
	}
	else if( slope < -1 )//The slope is in the 3rd or 7th octect
	{
		return 4;
	}	
	else//No problems, the line is in the 1st or 5th octect
		return 0;
}

void fillSpecialCase1( double x0, double y0, double x1, double y1, int **Raster )
{
	for( int i = x0 ; i <= x1 ; i++ )
	{
		putPixel( i , y0 , Raster );
	}
}

void fillSpecialCase2( double x0, double y0, double x1, double y1, int **Raster )
{
	for( int i = y0 ; i <= y1 ; i++ )
	{
		putPixel( x0 , i , Raster );
	}
}

void fillRasterNaive( double x0, double y0, double x1, double y1, double slope, double b, int **Raster, int evaluatedSlope )
{
	putPixel( x0 , y0 , Raster );
	putPixel( x1 , y1 , Raster );
	for( int i = x0 + 1 ; i < x1 ; i++ )
	{
		y0 = slope*i + b ;
		putPixel( i , (int)y0 , Raster );
	}
}

void fillRasterDDA( double x0, double y0, double x1, double y1, double slope, int **Raster, int evaluatedSlope )
{
	putPixel( x0 , y0 , Raster );
	putPixel( x1 , y1 , Raster );
	for( int i = x0 + 1 ; i < x1 ; i++ )
	{
		y0 = y0 + slope;
		putPixel( i , (int)y0 , Raster );
	}
}

void fillRasterBresenham( int x0, int y0, int x1, int y1, int **Raster, int evaluatedSlope )
{
	int dx;
	int dy;
	int d;
	int incE;
	int incNE;
	dx = x1 - x0;
	dy = y1 - y0;
	d = 2*dy - dx;
	incE = 2*dy;
	incNE = 2*( dy - dx );
	putPixel( x0 , y0 , Raster );
	putPixel( x1 , y1 , Raster );
	for( int i = x0 + 1 ; i < x1 ; i++ )
	{
		if( d <= 0 )
		{
			d+= incE;
		}
		else
		{
			d+= incNE;
			y0++;
		}
		putPixel( i , y0 , Raster );
	}
}

void putPixel( int x, int y, int **Raster )
{
	Raster[x][y]=1;
}
void drawLine( int **Raster, int width, int height, int evaluatedSlope )
{
	if( evaluatedSlope == 0 )
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
}
