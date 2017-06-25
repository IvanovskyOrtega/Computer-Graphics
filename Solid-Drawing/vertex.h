#ifndef _VERTEX_H_
#define _VERTEX_H_

double **createZBuffer();
void
VLF(double alfa, double beta, double gamma, double sfX, double sfY, double sfZ,
    double tX, double tY, double tZ, char *name, char *rawFile);
int readLines(char *rawFile);
void vertexList(struct vertex *vertexes, int n, char *rawFile);
void vertexList2(struct vertex *vertexes, int n, char *rawFile);
void edgesList(struct edge *edges, struct vertex *vertexes, int n,
	       char *rawFile);
void facesList(struct face **faces, struct vertex *vertexes,
	       struct edge *edges, int n, char *rawFile);

#endif
