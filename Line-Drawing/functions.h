#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

void lineProgram();
void fillRaster(double x0, double y0, double x1, double y1, double slope, double b, int **Raster);
void putPixel(int x, int y, int **Raster);
void drawLine(int **Raster, int width, int height);

#endif
