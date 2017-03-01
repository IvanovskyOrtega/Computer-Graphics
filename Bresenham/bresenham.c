#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"bresenham.h"

void bresenham()
{	
	unsigned char*** Raster;
	unsigned char rgb[3]={};
	double Points[4]={};
	int i;
	int j;
	double slope = 0;
	double b = 0;
	double aux;
	int evaluatedSlope;
	srand(clock());
	Raster = (unsigned char ***)malloc(1920*sizeof(unsigned char**));
	for (i = 0; i < 1920; ++i)
    	{
       	 	Raster[i] = (unsigned char **)malloc(1080 * sizeof(unsigned char *));
        	for (j = 0; j < 1080; ++j)
        	{
            		Raster[i][j] = (unsigned char*)malloc(3*sizeof(unsigned char));
        	}
    	}
	/*for( i = 0 ; i<10000 ; i++ )
	{*/
	rgb[0]=rand()%256;//Random color for the line
	rgb[1]=rand()%256;
	rgb[2]=rand()%256;
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
		fillSpecialCase1( Points[0], Points[1], Points[2], Points[3], Raster, rgb );	
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
		fillSpecialCase2( Points[0], Points[1], Points[2], Points[3], Raster, rgb );	
	}
	else
	{
		slope = ( Points[3] - Points[1] )/( Points[2] - Points[0] );
		evaluatedSlope = evaluateSlope( slope );
		b = Points[1] - (slope*(Points[0]));
		printf("The line ecuation is: y = %lfx + %lf\n",slope,b);
		if( evaluatedSlope ==1 ) 		
		{
			fillSpecialCase3( Points[0], Points[1], Points[2], Points[3], Raster, rgb );
		}
		else if( evaluatedSlope == 2 )
		{
			aux = Points[0];
			Points[0] = Points[1];
			Points[1] = aux;
			aux = Points[2];
			Points[2] = Points[3];
			Points[3] = aux;
			fillRasterBresenham( (int)Points[0], (int)Points[1], (int)Points[2], (int)Points[3], Raster, evaluatedSlope, rgb );
		}
		else if( evaluatedSlope == 3 )
		{
			aux = Points[1];
			Points[1] = Points[3];
			Points[3] = aux;	
			fillRasterBresenham( (int)Points[0], (int)Points[1], (int)Points[2], (int)Points[3], Raster, evaluatedSlope, rgb );	
		}
		else if( evaluatedSlope == 4 )
		{
			aux = Points[0];
			Points[0] = Points[3];
			Points[3] = Points[2];
			Points[2] = Points[1];
			Points[1] = aux;
			fillRasterBresenham( (int)Points[0], (int)Points[1], (int)Points[2], (int)Points[3], Raster, evaluatedSlope, rgb );
		}
		else if( evaluatedSlope == 5 )
		{
			fillSpecialCase4( Points[0], Points[1], Points[2], Points[3], Raster, rgb );	
		}
	}
	//}
	
	drawLine( Raster, 1920, 1080 );
	free(Raster);
	Raster = NULL;
}

double* getXYValues( double* Points )
{
	double x0;// = rand()%1920;
	double y0;// = rand()%1080;
	double x1;// = rand()%1920;
	double y1;// = rand()%1080;
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

int evaluateSlope( double slope )
{
	if( slope > 1 )//The line is in the 2nd or 6th octect
		return 2;
	else if( slope < 0 && slope > -1 )//The line is in the 4th or 8th octect
		return 3;
	else if( slope < -1 )//The slope is in the 3rd or 7th octect
		return 4;
	else if( slope == 1 )//45 degrees positive line
		return 1;
	else if( slope == -1 )//45 degrees negative line 
		return 5;
	else//No problems, the line is in the 1st or 5th octect
		return 0;
}

void fillSpecialCase1( double x0, double y0, double x1, double y1, unsigned char*** Raster, unsigned char* rgb )
{
	for( int i = x0 ; i <= x1 ; i++ )
	{
		putPixel( i , y0 , Raster, rgb );
	}
}

void fillSpecialCase2( double x0, double y0, double x1, double y1, unsigned char*** Raster, unsigned char* rgb )
{
	for( int i = y0 ; i <= y1 ; i++ )
	{
		putPixel( x0 , i , Raster, rgb );
	}
}

void fillSpecialCase3( double x0, double y0, double x1, double y1, unsigned char*** Raster, unsigned char* rgb )
{
	for( int i = x0 ; i <= x1 ; i++ )
	{
		putPixel( i , y0 , Raster, rgb );
		y0++;
	}
}

void fillSpecialCase4( double x0, double y0, double x1, double y1, unsigned char*** Raster, unsigned char* rgb )
{
	for( int i = x0 ; i <= x1 ; i++ )
	{
		putPixel( i , y1 , Raster, rgb );
		y1--;
	}
}

void fillRasterBresenham( int x0, int y0, int x1, int y1, unsigned char*** Raster, int evaluatedSlope, unsigned char* rgb )
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
	if( evaluatedSlope == 0 )
	{
		putPixel( x0 , y0 , Raster, rgb );
		putPixel( x1 , y1 , Raster, rgb );
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
			putPixel( i , y0 , Raster, rgb );
		}
	}
	else if( evaluatedSlope == 2 )
	{
		putPixel( y0 , x0 , Raster, rgb );
		putPixel( y1 , x1 , Raster, rgb );
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
			putPixel( y0 , i , Raster, rgb );
		}
	}
	else if( evaluatedSlope == 3 )
	{
		putPixel( x0 , y1 , Raster, rgb );
		putPixel( x1 , y0 , Raster, rgb );
		for( int i = x0 + 1 ; i < x1 ; i++ )
		{
			if( d <= 0 )
			{
				d+= incE;
			}
			else
			{
				d+= incNE;
				y1--;
			}
			putPixel( i , y1 , Raster, rgb );
		}
	}
	else
	{
		putPixel( y0 , x1 , Raster, rgb );
		putPixel( y1 , x0 , Raster, rgb );
		for( int i = (x1 - 1) ; i > x0 ; i-- )
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
			putPixel( y0 , i , Raster, rgb );
		}
	}
}

void putPixel( int x, int y, unsigned char*** Raster, unsigned char* rgb )
{
	Raster[x][y][0]=rgb[0];
	Raster[x][y][1]=rgb[1];
	Raster[x][y][2]=rgb[2];
}
void drawLine( unsigned char*** Raster, int width, int height )
{
	FILE* line;
	line = fopen("line.ppm","w");//Creates the file (if not exists), and overwrite it (if exists)
	fprintf(line,"P3\n");
	fprintf(line,"1920 1080\n");
	fprintf(line,"255\n");
	for( int i = height - 1 ; i >=0 ; i-- )
	{
		for( int j = 0 ; j < width ; j++ )
		{
			fprintf(line," %d %d %d\t",Raster[j][i][0],Raster[j][i][1],Raster[j][i][2]);
		}
		fprintf(line,"\n");
	}
	fclose(line);
}
