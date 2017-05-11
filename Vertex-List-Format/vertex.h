#ifndef _VERTEX_H_
#define _VERTEX_H_

struct vertex
{
  int num;
  double x;
  double y;
  double z;
  double hash;
} vertex;

struct edge
{
  int num;
  struct vertex vertex1;
  struct vertex vertex2;
  double hash;
} edge;

struct face
{
  int num;
  struct edge edge1;
  struct edge edge2;
  struct edge edge3;
} face;

void VLF ();
void generateVLF ();
void generateELF ();
void generateFLF ();

#endif
