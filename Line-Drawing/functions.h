#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

void lineProgram();
int** createRaster( int** Raster, int width, int height );
int** emptyRaster( int** Raster, int width, int height );
double* getXYValues( double* Points );
int evaluateSlope( double slope, double* Points );
void fillSpecialCase1( double x0, double y0, double x1, double y1, int **Raster );
void fillSpecialCase2( double x0, double y0, double x1, double y1, int **Raster );	
void fillRasterDDA( double x0, double y0, double x1, double y1, double slope, int **Raster, int evaluatedSlope );
void fillRasterNaive( double x0, double y0, double x1, double y1, double slope, double b, int **Raster, int evaluatedSlope );
void fillRasterBresenham( int x0, int y0, int x1, int y1, int **Raster, int evaluatedSlope );
void putPixel( int x, int y, int **Raster );
void drawLine( int **Raster, int width, int height, int evaluatedSlope );

#endif
