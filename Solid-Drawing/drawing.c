#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"bresenham.h"
#include"vertex.h"
#include"drawing.h"

double diffuseLight[3];
struct vertex L;
double cosineTheta, magnitude, dotProduct, finalCos;

/*
  This function determines if a face is gonna be drawn or not.
*/
double faceHidding(struct face f, struct vertex *vertexes, struct edge *edges)
{
	struct vertex A;
	struct vertex B;
	struct vertex C;
	double normalx = 0.0, normaly = 0.0, normalz = 0.0;
	double cosine;
	A = vertexes[edges[f.edge1->num].vertex1->num];	// Define the 3 vectors
	B = vertexes[edges[f.edge2->num].vertex1->num];
	C = vertexes[edges[f.edge3->num].vertex1->num];
	/* Calculate the normal vector */
	normalx = (((B.y - A.y) * (C.z - B.z)) - ((B.z - A.z) * (C.y - B.y)));
	normaly = (((B.z - A.z) * (C.x - B.x)) - ((B.x - A.x) * (C.z - B.z)));
	normalz = (((B.x - A.x) * (C.y - B.y)) - ((B.y - A.y) * (C.x - B.x)));
	/* Calculate the cosine */
	cosine =
	    ((normalz * -1) /
	     sqrtl((normalx * normalx) + (normaly * normaly) +
		   (normalz * normalz)));
//  printf ("COS: %lf\n", cosine);
	double angle = acos(cosine) * (180 / PI);
	//printf ("%lf\n", angle);
	return angle;
}

/*
  This fucntion fills the emply pixels of a face.
*/
void
scanline(struct pixels ***Raster, double **ZBuffer, unsigned char *rgb,
	 struct vertex normal, struct equation planeEquation)
{
	int x1, x2, flag = 0;
	double zb1, zb2, incZB, zbuff;
	diffuseLight[0] = 500;
	diffuseLight[1] = 100;
	diffuseLight[2] = 100;
	L.x = normal.x - diffuseLight[0];
	L.y = normal.y - diffuseLight[1];
	L.z = normal.z - diffuseLight[2];
	dotProduct = (normal.x * L.x) + (normal.y * L.y) + (normal.z * L.z);
	magnitude =
	    sqrt((normal.x * normal.x) + (normal.y * normal.y) +
		 (normal.z * normal.z)) * sqrt((L.x * L.x) + (L.y * L.y) +
					       (L.z * L.z));
	cosineTheta = dotProduct / magnitude;
	/*  finalCos = min(cosineTheta);
	   rgb[0] = max(cosineTheta+8*rgb[0]);
	   rgb[1] = max(cosineTheta+8*rgb[1]);
	   rgb[2] = max(cosineTheta+8*rgb[2]); */
	//printf ("%lf\n", cosineTheta);
	for (int i = 0; i < 1080; i++) {	/* Start to move over the Y axis */
		flag = 0;	/* Reset the flag for each i loop */
		for (int j = 0; j < 1920; j++) {	/* Star to move over the X axis */
			if (ZBuffer[j][i] > -10000) {	/* If a value in the ZBuffer is bigger than -10000 */
				if (j < 1919) {
					if (ZBuffer[j + 1][i] == -10000) {	/* Check if the next one is empty */
						if (flag == 0) {	/* Stablish the first limit */
							//zb1 = ZBuffer[j][i];
							x1 = j;
							flag = 1;
						} else if (flag == 1) {	/* Stablish the second limit */
							//zb2 = ZBuffer[j][i];
							x2 = j;
							//incZB = ((zb2 - zb1) / (x2 - x1));
							flag = 2;
							j = x1;
						} else if (flag == 2) {	/* We've achieved the limit */
							break;
						}
					}
				}
			} else if (flag == 2) {	/* Fill the blank pixels */
				zbuff =
				    (-(planeEquation.a * j) -
				     (planeEquation.b * i) -
				     planeEquation.d) / planeEquation.c;
				if (zbuff > ZBuffer[j][i]) {
					/* Illumination hasn't been implemented. */
					putPixel(j, i, Raster, rgb, normal.z);
					//zb1 += incZB;
					ZBuffer[j][i] = zbuff;
					Raster[j][i]->zBuffer = zbuff;
				}
			}
		}
	}
}

double min(double a)
{
	if (a < 0.0)
		return 0.0;
	else
		return a;
}

unsigned char max(double a)
{
	if (a > 255.0)
		return 255;
	else
		return (unsigned char)a;
}
