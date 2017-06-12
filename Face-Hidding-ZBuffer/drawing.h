#ifndef _DRAWING_H_
#define _DRAWING_H_

#define PI 3.14159265

double faceHidding (struct face f, struct vertex *vertexes,
		    struct edge *edges);
void scanline (struct pixels ***Raster, double **ZBuffer, unsigned char *rgb,
	       struct vertex normal);

#endif
