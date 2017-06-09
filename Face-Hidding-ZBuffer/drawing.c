#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"bresenham.h"
#include"vertex.h"
#include"drawing.h"

double
faceHidding (struct face f, struct vertex *vertexes, struct edge *edges)
{
  struct vertex A;
  struct vertex B;
  struct vertex C;
  double normalx = 0.0, normaly = 0.0, normalz = 0.0;
  double cosine;
  A = vertexes[edges[f.edge1->num].vertex1->num]; // Define the 3 vectors
  B = vertexes[edges[f.edge2->num].vertex1->num];
  C = vertexes[edges[f.edge3->num].vertex1->num];
  /* Calculate the normal vector */
  normalx = (((B.y - A.y) * (C.z - B.z)) - ((B.z - A.z) * (C.y - B.y)));
  normaly = (((B.z - A.z) * (C.x - B.x)) - ((B.x - A.x) * (C.z - B.z)));
  normalz = (((B.x - A.x) * (C.y - B.y)) - ((B.y - A.y) * (C.x - B.x)));
  /* Calculate the cosine */
  cosine =
    ((normalz * -1) /
     sqrtl ((normalx * normalx) + (normaly * normaly) + (normalz * normalz)));
//  printf ("COS: %lf\n", cosine);
/* Also can calculate the angle, but it is not as efficient as just using the
  cosine, and using the angle it's not very accurate.
  double angle = acos (cosine) * (180 / PI);
  printf ("%lf\n", angle);
  */
  return cosine;
}

void
scanline (struct pixels ***Raster, double **ZBuffer, int r, int g, int b)
{
  int start, end, limit = 0;
  double zb1, zb2, incZB;
  for (int i = 0; i < 1080; i++)	/* Start to move over the Y axis */
    {
      limit = 0;			/* Reset the limit for each i loop */
      for (int j = 0; j < 1920; j++)	/* Star to move over the X axis */
	{
	  if (ZBuffer[j][i] > -10000)	/* If a value in the ZBuffer is bigger than -10000 */
	    {
	      if (j < 1919)
		{
		  if (ZBuffer[j + 1][i] == -10000)	/* Check if the next one is empty */
		    {
		      if (limit == 0)	/* Stablish the first limit */
			{
			  zb1 = ZBuffer[j][i];
			  start = j;
			  limit = 1;
			}
		      else if (limit == 1)	/* Stablish the second limit */
			{
			  zb2 = ZBuffer[j][i];
			  end = j;
			  incZB = ((zb2 - zb1) / (end - start));
			  limit = 2;
			  j = start;
			}
		      else if (limit == 2)	/* We've achieved the limit */
			{
			  break;
			}
		    }
		}
	    }
	  else if (limit == 2)	/* Fill the blank pixels */
	    {
	      if (zb1 >= Raster[j][i]->zBuffer)
		{
		  Raster[j][i]->rgb[0] = r;
		  Raster[j][i]->rgb[1] = g;
		  Raster[j][i]->rgb[2] = b;
		  zb1 += incZB;
		  ZBuffer[j][i] = zb1;
		  Raster[j][i]->zBuffer = zb1;
		}
	    }
	}
    }
}
