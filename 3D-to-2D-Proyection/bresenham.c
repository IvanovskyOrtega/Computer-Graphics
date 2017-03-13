#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"bresenham.h"

static const double f = 200;

void threeDtotwoD()
{
	int n = 0;
	unsigned char*** Raster=NULL;
	double sumX = 0;
	double sumY = 0;
	double sumZ = 0;
	double cX;
	double cY;
	double cZ;
	char c;
	Raster = createRaster();
	FILE *raw = fopen("model.raw","r");
	while ((c = fgetc(raw)) != EOF)
    	if (c == '\n')
    		n+=3;
	struct point *Points = NULL;
	rewind(raw);
	Points = ( struct point *)malloc( 3*n*sizeof( struct point ) );
	for( int i = 0 ; i < n ; i++ )
	{
		fscanf(raw, "%lf %lf %lf ",&Points[i].x, &Points[i].y,&Points[i].z );
		Points[i].w = 1.000;
		sumX = sumX + Points[i].x;
		sumY = sumY + Points[i].y;
		sumZ = sumZ + Points[i].z;
	}
	cX = sumX / n ;
	cY = sumY / n ;
	cZ = sumZ / n ;
	/*
	for( int i = 0 ; i < n ; i++ )
	{
		printf("\nPoint[ %d ] coordinate x = %lf",i+1,Points[i].x);
		printf("\nPoint[ %d ] coordinate y = %lf",i+1,Points[i].y);
		printf("\nPoint[ %d ] coordinate z = %lf",i+1,Points[i].z);
		printf("\nPoint[ %d ] coordinate w = %lf",i+1,Points[i].w);
	}
	printf("\nThe center of the object is: (%lf,%lf,%lf)",cX,cY,cZ);
	*/
	fclose( raw );
	translateAndProyect( n, Points, cX, cY, cZ );
	transform3D(n,Points);
	scaleAndTranslate( n, Points, cX, cY, cZ  );
	mainBresenham( n, Points, Raster );
	free( Points );
	Points = NULL;
	free(Raster);
	Raster = NULL;
}

void translateAndProyect( int n, struct point *Points, double cX, double cY, double cZ )
{
	double matrix[4][4]={ {1,0,0,-cX},
                        {0,1,0,-cY},
                        {0,0,1,(3*f)-cZ},
                        {0,0,1/f,(((3*f)-cZ))/f} };
  int i=0;
  int j=0;
  int op=1;
  struct point aux;
  for(i=0;i<n;i++)
  {
    aux.x=0.0;
    aux.y=0.0;
    aux.z=0.0;
    aux.w=0.0;
    for(j=0;j<4;j++)
    {
      switch(op)
      {
        case 1:
          aux.x+=(matrix[j][0]*Points[i].x);
          aux.x+=(matrix[j][1]*Points[i].y);
          aux.x+=(matrix[j][2]*Points[i].z);
          aux.x+=(matrix[j][3]*Points[i].w);
          op=2;
        break;
        case 2:
          aux.y+=(matrix[j][0]*Points[i].x);
          aux.y+=(matrix[j][1]*Points[i].y);
          aux.y+=(matrix[j][2]*Points[i].z);
          aux.y+=(matrix[j][3]*Points[i].w);
          op=3;
        break;
        case 3:
          aux.z+=(matrix[j][0]*Points[i].x);
          aux.z+=(matrix[j][1]*Points[i].y);
          aux.z+=(matrix[j][2]*Points[i].z);
          aux.z+=(matrix[j][3]*Points[i].w);
          op=4;
        break;
        case 4:
          aux.w+=(matrix[j][0]*Points[i].x);
          aux.w+=(matrix[j][1]*Points[i].y);
          aux.w+=(matrix[j][2]*Points[i].z);
          aux.w+=(matrix[j][3]*Points[i].w);
          op=1;
        break;
      }
    }
    Points[i]=aux;
  }
}

void transform3D(int n, struct point *Points)
{
  int i=0;
  for(i=0;i<n;i++)
  {
    Points[i].x/=Points[i].w;
    Points[i].y/=Points[i].w;
    Points[i].z/=Points[i].w;
  }

}

void scaleAndTranslate( int n, struct point * Points, double cX, double cY, double cZ )
{
	double tX = 960 + cX;
	double tY = 540 + cY;
	for(int i=0;i<n;i++)
  {
    Points[i].x*=1000;
    Points[i].y*=1000;
    Points[i].x+=tX;
    Points[i].y+=tY;
  }
	for(int i=0;i<n;i++)
    printf("point in the 3D form(%lf,%lf,%lf)\n",Points[i].x,Points[i].y,Points[i].z);
}

unsigned char *** createRaster()
{
	unsigned char *** Raster;
	Raster = (unsigned char ***)malloc(1920*sizeof(unsigned char**));
	for ( int i = 0 ; i < 1920 ; i++ )
    	{
       	 	Raster[i] = (unsigned char **)malloc(1080 * sizeof(unsigned char *));
        	for ( int j = 0 ; j < 1080 ; j++ )
        	{
            		Raster[i][j] = (unsigned char*)malloc(3*sizeof(unsigned char));
								Raster[i][j][0]=255;
								Raster[i][j][1]=255;
								Raster[i][j][2]=255;
        	}
    	}
			return Raster;
}

void mainBresenham( int n, struct point *Points, unsigned char *** Raster )
{
	//for(int i=0;i<n;i++)
    //printf("In bresenham point in the 3D form(%lf,%lf,%lf)\n",Points[i].x,Points[i].y,Points[i].z);
	int i;
	unsigned char rgb[3]={0,0,255};
	for( i = 0 ; i < n-3 ; i+=3 )
	{
		drawBresenham( Points[i].x, Points[i].y, Points[i+1].x, Points[i+1].y, Raster, rgb );
		drawBresenham( Points[i+1].x, Points[i+1].y, Points[i+2].x, Points[i+2].y, Raster, rgb );
		drawBresenham( Points[i+2].x, Points[i+2].y, Points[i].x, Points[i].y, Raster, rgb );
	}
	drawLine( Raster, 1920, 1080 );
}

void drawBresenham( double x0, double y0, double x1, double y1, unsigned char *** Raster, unsigned char * rgb )
{
	double Points[4]={};
	double slope = 0;
	double b = 0;
	double aux;
	int evaluatedSlope;
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
		if(Points[3] - Points[1] == 0)
		{
			if(Points[2] < Points[0])
			{
				aux = Points[0];
				Points[0] = Points[2];
				Points[2] = aux;
			}
			printf("\nSpecial case, the algorithm isn't going to be used, but the the program will draw the line :D\n");
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
			printf("\nSpecial case, the algorithm isn't going to be used, but the the program will draw the line :D\n");
			printf("The line ecuation is: x = %lf\n",Points[0]);
			fillSpecialCase2( Points[0], Points[1], Points[2], Points[3], Raster, rgb );
		}
		else
		{
			slope = ( Points[3] - Points[1] )/( Points[2] - Points[0] );
			evaluatedSlope = evaluateSlope( slope );
			b = Points[1] - (slope*(Points[0]));
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
				fillRasterBresenham( Points[0], Points[1], Points[2], Points[3], Raster, evaluatedSlope, rgb );
			}
			else if( evaluatedSlope == 3 )
			{
				aux = Points[1];
				Points[1] = Points[3];
				Points[3] = aux;
				fillRasterBresenham( Points[0], Points[1], Points[2], Points[3], Raster, evaluatedSlope, rgb );
			}
			else if( evaluatedSlope == 4 )
			{
				aux = Points[0];
				Points[0] = Points[3];
				Points[3] = Points[2];
				Points[2] = Points[1];
				Points[1] = aux;
				fillRasterBresenham( Points[0], Points[1], Points[2], Points[3], Raster, evaluatedSlope, rgb );
			}
			else if( evaluatedSlope == 5 )
			{
				fillSpecialCase4( Points[0], Points[1], Points[2], Points[3], Raster, rgb );
			}
			else
			{
				fillRasterBresenham( (int)Points[0], (int)Points[1], (int)Points[2], (int)Points[3], Raster, evaluatedSlope, rgb );
			}
		}
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
		putPixel( i , y0 , Raster, rgb );
		y0--;
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
