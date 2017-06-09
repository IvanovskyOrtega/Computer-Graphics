#ifndef _VERTEX_H_
#define _VERTEX_H_

void VLF (double alfa, double beta, double gamma, double sfX, double sfY,
	  double tX, double tY, char *name);
int readLines ();
void vertexList (struct vertex *vertexes, int n);
void vertexList2 (struct vertex *vertexes, int n);
void edgesList (struct edge *edges, struct vertex *vertexes, int n);
void facesList (struct face **faces, struct vertex *vertexes,
		struct edge *edges, int n);
int numberOfVertexes (struct vertex *vertexes, int n);
int numberOfEdges (struct edge *edges, int n);
int numberOfFaces (struct face *faces, int n);
double **createZBuffer ();

#endif
