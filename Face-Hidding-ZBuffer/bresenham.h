#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

struct pixels
{
  unsigned char rgb[3];
  double zBuffer;
  double normal;
};

struct point
{
  double x;
  double y;
  double z;
  double w;
} Point;

struct vertex
{
  int num;
  double x;
  double y;
  double z;
  double zb;
  double w;
  double hash;
};

struct edge
{
  int num;
  struct vertex *vertex1;
  struct vertex *vertex2;
  double hash;
};

struct face
{
  int num;
  struct edge *edge1;
  struct edge *edge2;
  struct edge *edge3;
};

void threeDtotwoD (double alfa, double beta, double gamma, char *name);
struct pixels ***createRaster ();
void cleanRaster (struct pixels ****Raster);
void cleanZBuffer (double ***ZBuffer);
struct vertex getFaceNormal (struct face f, struct vertex *vertexes,
			     struct edge *edges);
void translation (int n, struct vertex *vertexes, double cX, double cY,
		  double cZ);
void rotationX (struct vertex *vertexes, int n, double alfa);
void rotationY (struct vertex *vertexes, int n, double beta);
void rotationZ (struct vertex *vertexes, int n, double gamma);
void
returnTranslation (struct vertex *vertexes, double cX, double cY, double cZ,
		   int n);
void applyMatrix (struct vertex *vertexes, int n, double matrix[4][4]);
void applyMatrixRT (struct vertex *vertexes, int n, double matrix[4][4]);
void transform3D (int n, struct vertex *vertexes);
void scaleAndTranslate (int n, struct vertex *vertexes, double cX, double cY,
			double cZ, double sfX, double sfY, double tX,
			double tY);
void mainBresenham (int n, struct face *faces, struct edge *edges,
		    struct vertex *vertexes, struct pixels ***Raster,
		    char *name);
int evaluateSlope (double slope);
void
drawBresenham (double x0, double y0, double x1, double y1, double zb1,
	       double zb2, struct pixels ***Raster, unsigned char *rgb,
	       double **ZBuffer, double normal);
void fillSpecialCase1 (int x0, int y0, int x1, int y1,
		       struct pixels ***Raster, unsigned char *rgb,
		       double zb1, double zb2, double **ZBuffer,
		       double normal);
void fillSpecialCase2 (int x0, int y0, int x1, int y1,
		       struct pixels ***Raster, unsigned char *rgb,
		       double zb1, double zb2, double **ZBuffer,
		       double normal);
void fillSpecialCase3 (int x0, int y0, int x1, int y1,
		       struct pixels ***Raster, unsigned char *rgb,
		       double zb1, double zb2, double **ZBuffer,
		       double normal);
void fillSpecialCase4 (int x0, int y0, int x1, int y1,
		       struct pixels ***Raster, unsigned char *rgb,
		       double zb1, double zb2, double **ZBuffer,
		       double normal);
void fillRasterBresenham (int x0, int y0, int x1, int y1,
			  struct pixels ***Raster, int evaluatedSlope,
			  unsigned char *rgb, double zb1, double zb2,
			  double **ZBuffer, double normal);
void translateAndProyect (int n, struct vertex *vertexes, double cX,
			  double cY, double cZ);
void putPixel (int x, int y, struct pixels ***Raster, unsigned char *rgb,
	       double normal);
void generateImage (struct pixels ***Raster, int width, int height,
		    char *name);

#endif
