#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "bresenham.h"
#include "vertex.h"
#include "drawing.h"

int j = 0;			/* Main vertex list counter */
int m = 0;			/* Edges list counter */
int fa = 0;			/* Faces list counter */
int vlc = 0;			/* Auxiliar vertex list counter */

/*
  This function performs almost everything in the program (calling
  other function of the other libraries). Gets the vertex list,
  edges list, faces list. Calls the transformation functions in
  bresenham.h
*/
void
VLF(double alfa, double beta, double gamma, double sfX, double sfY, double sfZ,
    double tX, double tY, double tZ, char *name, char *rawFile)
{
	srand(clock());
	int n;
	struct pixels ***Raster = NULL;
	double sumX = 0;
	double sumY = 0;
	double sumZ = 0;
	printf("Your parameters:\n");
	printf("Alfa=%lf\nBeta=%lf\nGamma=%lf\n", alfa, beta, gamma);
	printf("X cooordinate scale factor: %lf\n", sfX);
	printf("Y cooordinate scale factor: %lf\n", sfY);
	printf("X cooordinate translation: %lf\n", tX);
	printf("Y cooordinate translation: %lf\n", tY);
	printf("Output filename: %s.ppm\n", name);
	double cX;
	double cY;
	double cZ;
	Raster = createRaster();
	struct point Point;
	struct vertex *vertexes;	// Vertexes array
	struct vertex *vL;
	struct edge *edges;	// Edges array
	struct face *faces;	// Faces array
	n = readLines(rawFile);
	vL = (struct vertex *)malloc(n * sizeof(struct vertex));
	vertexes = (struct vertex *)malloc(n * sizeof(struct vertex));	// Allocate memory for the vertexes array
	edges = (struct edge *)malloc(n * sizeof(struct edge));	// Allocate memory for the edges array
	faces = (struct face *)malloc((n / 3) * sizeof(struct face));	// Allocate memory for the faces array
	vertexList(vertexes, n, rawFile);
	vertexList2(vL, n, rawFile);	/* The second list is needes to don't transform the vertexes to 2D */
	edgesList(edges, vertexes, n, rawFile);
	facesList(&faces, vertexes, edges, n, rawFile);
	FILE *raw = fopen(rawFile, "r");
	for (int i = 0; i < n; i++) {
		fscanf(raw, "%lf %lf %lf ", &Point.x, &Point.y, &Point.z);
		sumX = sumX + Point.x;
		sumY = sumY + Point.y;
		sumZ = sumZ + Point.z;
		Point.x = 0;
		Point.y = 0;
		Point.z = 0;
	}
	cX = sumX / n;
	cY = sumY / n;
	cZ = sumZ / n;
	printf("cX:%lf cY:%lf cZ:%lf\n", cX, cY, cZ);
	fclose(raw);
	/* Apply needed transformations for the auxiliar vertexes array */
	translation(vlc, vL, cX, cY, cZ);
	rotationX(vL, vlc, alfa);
	rotationY(vL, vlc, beta);
	rotationZ(vL, vlc, gamma);
	returnTranslation(vL, cX, cY, cZ, vlc);
	scale(vlc, vL, sfX, sfY, sfZ);
	translate(vlc, vL, tX, tY, tZ);
	/* Apply all the transformations for the main vertees array */
	translation(j, vertexes, cX, cY, cZ);
	rotationX(vertexes, j, alfa);
	rotationY(vertexes, j, beta);
	rotationZ(vertexes, j, gamma);
	returnTranslation(vertexes, cX, cY, cZ, j);
	translateAndProyect(j, vertexes, cX, cY, cZ);
	scale(j, vertexes, sfX, sfY, sfZ);
	translate(j, vertexes, tX, tY, tZ);
	transform3D(j, vertexes);
	mainBresenham(fa, faces, edges, vL, Raster, name);
	//free (vertexes);
	//vertexes = NULL;
	//free (Raster);
	//Raster = NULL;
}

/*
  This function creates the ZBuffer matrix initialized with
  bigger negative values (-10000 in this case).
*/
double **createZBuffer()
{
	double **ZBuffer = NULL;
	ZBuffer = (double **)malloc(1920 * sizeof(double *));
	for (int i = 0; i < 1920; i++) {
		ZBuffer[i] = (double *)malloc(1080 * sizeof(double));
	}
	for (int i = 0; i < 1920; i++)
		for (int j = 0; j < 1080; j++)
			ZBuffer[i][j] = -10000.0;
	return ZBuffer;
}

/*
  This function read how many lines does the raw file have.
*/
int readLines(char *rawFile)
{
	char c;
	int n = 0;
	FILE *raw = fopen(rawFile, "r");	// File pointer to model.raw file
	while ((c = fgetc(raw)) != EOF)	// Loop to count number of lines in the raw file
		if (c == '\n')	// If theres a line break
			n += 3;	// n+=3, since there are 3 vertexes per line
	fclose(raw);
	return n;
}

/*
  This function gets the vertex list using the raw file.
*/
void vertexList(struct vertex *vertexes, int n, char *rawFile)
{
	int i;			// Just a counter
	int k;			// Just a counter
	int exist = 0;		// Used for know if a vertex already exist in the vertexes array
	struct vertex aux;	// Auxiliar vertex
	FILE *raw = fopen(rawFile, "r");	// File pointer to model.raw file
	for (i = 0; i < n; i++)	// Loop to read the vertexes from the file
	{
		fscanf(raw, "%lf %lf %lf ", &aux.x, &aux.y, &aux.z);	// Scan data from the raw file
		aux.hash = aux.x + aux.y + aux.z;	// Set the aux hash value
		aux.zb = aux.z;
		aux.w = 1;
		aux.num = j;
		if (i > 0)	// If we are reading a value that it's not the first
		{
			for (k = 0; k < j; k++)	// Loop to compare hash values between the auxiliar vertex and each vertex in the vertexes array
			{
				if (aux.hash == vertexes[k].hash)	// If we find a vertex with the same hash
				{
					if (aux.x == vertexes[k].x && aux.y == vertexes[k].y && aux.z == vertexes[k].z)	// If it has the same x,y,x values
					{
						exist = 1;	// The vertex is already in the array
						break;	// break the loop
					}
				}
			}
		}
		if (exist != 1)	// If the vertex isn't in the array
		{
			vertexes[j] = aux;	// Add the vertex to the array
			j++;	// Increase the vertexes counter
		}
		exist = 0;	// Reset the exist value
	}
	/*
	   Print the vertexes
	   for (i = 0; i < j; i++)
	   {
	   printf ("Vertex %d x %lf y %lf z %lf zb%lf\n", vertexes[i].num,
	   vertexes[i].x, vertexes[i].y, vertexes[i].z, vertexes[i].zb);
	   }
	 */
	fclose(raw);
}

/*
  This function gets the auxiliar vertex list using the raw file.
*/
void vertexList2(struct vertex *vertexes, int n, char *rawFile)
{
	int i;			// Just a counter
	int k;			// Just a counter
	int exist = 0;		// Used for know if a vertex already exist in the vertexes array
	struct vertex aux;	// Auxiliar vertex
	FILE *raw = fopen(rawFile, "r");	// File pointer to model.raw file
	for (i = 0; i < n; i++)	// Loop to read the vertexes from the file
	{
		fscanf(raw, "%lf %lf %lf ", &aux.x, &aux.y, &aux.z);	// Scan data from the raw file
		aux.hash = aux.x + aux.y + aux.z;	// Set the aux hash value
		aux.zb = aux.z;
		aux.w = 1;
		aux.num = vlc;
		if (i > 0)	// If we are reading a value that it's not the first
		{
			for (k = 0; k < vlc; k++)	// Loop to compare hash values between the auxiliar vertex and each vertex in the vertexes array
			{
				if (aux.hash == vertexes[k].hash)	// If we find a vertex with the same hash
				{
					if (aux.x == vertexes[k].x && aux.y == vertexes[k].y && aux.z == vertexes[k].z)	// If it has the same x,y,x values
					{
						exist = 1;	// The vertex is already in the array
						break;	// break the loop
					}
				}
			}
		}
		if (exist != 1)	// If the vertex isn't in the array
		{
			vertexes[vlc] = aux;	// Add the vertex to the array
			vlc++;	// Increase the vertexes counter
		}
		exist = 0;	// Reset the exist value
	}
	/*
	   Print the vertexes
	   for (i = 0; i < vlc; i++)
	   {
	   printf ("Vertex %d x %lf y %lf z %lf zb%lf\n", vertexes[i].num,
	   vertexes[i].x, vertexes[i].y, vertexes[i].z, vertexes[i].zb);
	   }
	 */
	fclose(raw);
}

/*
  This function gets the edges list using the raw file.
*/
void
edgesList(struct edge *edges, struct vertex *vertexes, int n, char *rawFile)
{
	int i = 0;
	int x = 0;
	int k = 0;
	struct vertex v1;
	struct vertex v2;
	struct vertex v3;
	struct vertex *a1 = NULL;
	struct vertex *a2 = NULL;
	struct vertex *a3 = NULL;
	struct edge e1;
	struct edge e3;
	struct edge e2;
	FILE *fp = fopen(rawFile, "r");
	for (i = 0, k = 0; i < n; i += 3) {
		fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf %lf ", &v1.x, &v1.y,
		       &v1.z, &v2.x, &v2.y, &v2.z, &v3.x, &v3.y, &v3.z);
		v1.w = 1.0;
		v1.hash = v1.x + v1.y + v1.z;
		v2.w = 1.0;
		v2.hash = v2.x + v2.y + v2.z;
		v3.w = 1.0;
		v3.hash = v3.x + v3.y + v3.z;
		for (x = 0; x < j; x++) {
			if (v1.hash == vertexes[x].hash) {
				if ((v1.x == vertexes[x].x)
				    && (v1.y == vertexes[x].y)
				    && (v1.z == vertexes[x].z)) {
					a1 = &vertexes[x];
					break;
				}
			}
		}
		for (x = 0; x < j; x++) {
			if (v2.hash == vertexes[x].hash) {
				if ((v2.x == vertexes[x].x)
				    && (v2.y == vertexes[x].y)
				    && (v2.z == vertexes[x].z)) {
					a2 = &vertexes[x];
					break;
				}
			}
		}
		for (x = 0; x < j; x++) {
			if (v3.hash == vertexes[x].hash) {
				if ((v3.x == vertexes[x].x)
				    && (v3.y == vertexes[x].y)
				    && (v3.z == vertexes[x].z)) {
					a3 = &vertexes[x];
					break;
				}
			}
		}
		e1.vertex1 = a1;
		e1.vertex2 = a2;
		e1.hash = (e1.vertex1->hash) + (e1.vertex2->hash);
		e2.vertex1 = a2;
		e2.vertex2 = a3;
		e2.hash = (e2.vertex1->hash) + (e2.vertex2->hash);
		e3.vertex1 = a3;
		e3.vertex2 = a1;
		e3.hash = (e3.vertex1->hash) + (e3.vertex2->hash);
		/*
		   This part was designed to do not repeat edges which are already in the
		   list. But if we do this, it's more difficult to know in which direction
		   our line it's going to be draw.
		   for (int l = 0; l < i; l++)
		   {
		   if (e1.hash == edges[l].hash)
		   {
		   if (((e1.vertex1 == edges[l].vertex1)
		   && (e1.vertex2 == edges[l].vertex2))
		   || ((e1.vertex1 == edges[l].vertex2)
		   && (e1.vertex2 == edges[l].vertex1)))
		   {
		   exist1 = 1;
		   break;
		   }
		   }
		   }
		   for (int l = 0; l < i; l++)
		   {
		   if (e2.hash == edges[l].hash)
		   {
		   if (((e2.vertex1 == edges[l].vertex1)
		   && (e2.vertex2 == edges[l].vertex2))
		   || ((e2.vertex1 == edges[l].vertex2)
		   && (e2.vertex2 == edges[l].vertex1)))
		   {
		   exist2 = 1;
		   break;
		   }
		   }
		   }
		   for (int l = 0; l < i; l++)
		   {
		   if (e3.hash == edges[l].hash)
		   {
		   if (((e3.vertex1 == edges[l].vertex1)
		   && (e3.vertex2 == edges[l].vertex2))
		   || ((e3.vertex1 == edges[l].vertex2)
		   && (e3.vertex2 == edges[l].vertex1)))
		   {
		   exist3 = 1;
		   break;
		   }
		   }
		   }
		   if (exist1 == 0)
		   {
		 */
		edges[k].num = k;
		edges[k].vertex1 = e1.vertex1;
		edges[k].vertex2 = e1.vertex2;
		edges[k].hash = e1.hash;
		k++;
		m++;
		/*
		   }
		   if (exist2 == 0)
		   {
		 */
		edges[k].num = k;
		edges[k].vertex1 = e2.vertex1;
		edges[k].vertex2 = e2.vertex2;
		edges[k].hash = e2.hash;
		k++;
		m++;
		/*
		   }
		   if (exist3 == 0)
		   {
		 */
		edges[k].num = k;
		edges[k].vertex1 = e3.vertex1;
		edges[k].vertex2 = e3.vertex2;
		edges[k].hash = e3.hash;
		k++;
		m++;
//}
	}
	fclose(fp);
/*
for (i = 0; i < m; i++)
  {
    printf ("Edge %d Vertex %d Vertex %d\n", edges[i].num,
      edges[i].vertex1->num, edges[i].vertex2->num);
  }
  */
}

/*
  This function gets the faces list using the raw file.
*/
void
facesList(struct face **faces, struct vertex *vertexes, struct edge *edges,
	  int n, char *rawFile)
{
	int i;			// Just a counter
	int k;			// Just a counter
	struct vertex aux1;
	struct vertex aux2;
	struct vertex aux3;
	struct vertex *a1;
	struct vertex *a2;
	struct vertex *a3;
	struct edge e1;
	struct edge e2;
	struct edge e3;
	struct edge *ae1;	// Auxiliar edge
	struct edge *ae2;	// Auxiliar edge
	struct edge *ae3;	// Auxiliar edge
	FILE *raw = fopen(rawFile, "r");	// File pointer to model.raw file
	for (i = 0; i < (n / 3); i++)	// vertex1 to generate the Faces List
	{
		fscanf(raw, "%lf %lf %lf %lf %lf %lf %lf %lf %lf ", &aux1.x, &aux1.y, &aux1.z, &aux2.x, &aux2.y, &aux2.z, &aux3.x, &aux3.y, &aux3.z);	//Read all the line
		aux1.zb = aux1.z;
		aux1.w = 1.0;
		aux2.zb = aux2.z;
		aux2.w = 1.0;
		aux3.zb = aux2.z;
		aux3.w = 1.0;
		aux1.hash = aux1.x + aux1.y + aux1.z;
		aux2.hash = aux2.x + aux2.y + aux2.z;
		aux3.hash = aux3.x + aux3.y + aux3.z;
		for (k = 0; k < j; k++)	// Compare if the vertex a1 exists in the vertexes array
		{
			if (aux1.hash == vertexes[k].hash)	// If it has the same hash
			{
				if (aux1.x == vertexes[k].x && aux1.y == vertexes[k].y && aux1.z == vertexes[k].z)	// Compare if it's the same vertex
				{
					a1 = &vertexes[k];
					break;
				}
			}
		}
		for (k = 0; k < j; k++)	// Same as the a1 vertex
		{
			if (aux2.hash == vertexes[k].hash) {
				if (aux2.x == vertexes[k].x
				    && aux2.y == vertexes[k].y
				    && aux2.z == vertexes[k].z) {
					a2 = &vertexes[k];
					break;
				}
			}
		}
		for (k = 0; k < j; k++)	// Same as the a1 vertex
		{
			if (aux3.hash == vertexes[k].hash) {
				if (aux3.x == vertexes[k].x
				    && aux3.y == vertexes[k].y
				    && aux3.z == vertexes[k].z) {
					a3 = &vertexes[k];
					break;
				}
			}
		}
		e1.vertex1 = a1;
		e1.vertex2 = a2;
		e1.hash = aux1.hash + aux2.hash;
		e2.vertex1 = a2;
		e2.vertex2 = a3;
		e2.hash = aux2.hash + aux3.hash;
		e3.vertex1 = a3;
		e3.vertex2 = a1;
		e3.hash = aux3.hash + aux1.hash;
		for (k = 0; k < m; k++) {
			if (e1.hash == edges[k].hash) {
				/*if (e1.vertex1 == edges[k].vertex2
				   && e1.vertex2 == edges[k].vertex1)
				   {
				   ae1 = &edges[k];
				   break;
				   }
				   else */ if (e1.vertex1 == edges[k].vertex1
					       && e1.vertex2 ==
					       edges[k].vertex2) {
					ae1 = &edges[k];
					break;
				}
			}
		}
		for (k = 0; k < m; k++) {
			if (e2.hash == edges[k].hash) {	/*
							   if (e2.vertex1 == edges[k].vertex2
							   && e2.vertex2 == edges[k].vertex1)
							   {
							   ae2 = &edges[k];
							   break;
							   }
							   else */
				if (e2.vertex1 == edges[k].vertex1
				    && e2.vertex2 == edges[k].vertex2) {
					ae2 = &edges[k];
					break;
				}
			}
		}
		for (k = 0; k < m; k++) {
			if (e3.hash == edges[k].hash) {	/*
							   if (e3.vertex1 == edges[k].vertex2
							   && e3.vertex2 == edges[k].vertex1)
							   {
							   ae3 = &edges[k];
							   break;
							   }
							   else */
				if (e3.vertex1 == edges[k].vertex1
				    && e3.vertex2 == edges[k].vertex2) {
					ae3 = &edges[k];
					break;
				}
			}
		}
		(*faces)[fa].num = fa;
		(*faces)[fa].edge1 = ae1;
		(*faces)[fa].edge2 = ae2;
		(*faces)[fa].edge3 = ae3;
		fa++;
	}
	/*
	   Print faces list
	   for (i = 0; i < fa; i++)
	   {
	   printf ("Face %d Edge %d Edge %d Edge %d\n", (*faces)[i].num,
	   (*faces)[i].edge1->num, (*faces)[i].edge2->num,
	   (*faces)[i].edge3->num);
	   }
	 */
	fclose(raw);
}
