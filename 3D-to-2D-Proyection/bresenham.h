#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

struct point
{
	double x;
	double y;
	double z;
	double w;
}Point;

unsigned char *** createRaster();
void transform3D(int n, struct point *Points3DH);
void scaleAndTranslate( int n, struct point * Points, double cX, double cY, double cZ );
void mainBresenham( int n, struct point *Points2D, unsigned char *** Raster );
void drawBresenham( double x0, double y0, double x1, double y1, unsigned char *** Raster, unsigned char * rgb );
void threeDtotwoD();
void translateAndProyect( int n, struct point *Points3DH, double cX, double cY, double cZ );
int evaluateSlope( double slope );
void fillSpecialCase1( double x0, double y0, double x1, double y1, unsigned char*** Raster, unsigned char* rgb );
void fillSpecialCase2( double x0, double y0, double x1, double y1, unsigned char*** Raster, unsigned char* rgb );
void fillSpecialCase3( double x0, double y0, double x1, double y1, unsigned char*** Raster, unsigned char* rgb );
void fillSpecialCase4( double x0, double y0, double x1, double y1, unsigned char*** Raster, unsigned char* rgb );
void fillRasterBresenham( int x0, int y0, int x1, int y1, unsigned char*** Raster, int evaluatedSlope, unsigned char* rgb );
void putPixel( int x, int y, unsigned char*** Raster, unsigned char* rgb );
void drawLine( unsigned char*** Raster, int width, int height );

#endif
