#include <stdio.h>
#include <stdlib.h>
#include "vertex.h"

/**
  * Compilation instructions:
  * Linux:
  *   $ make                                #to compile
  *   $ make run                            #to compile and run
  * Windows:
  *   > gcc -c vertex.c main.c -Wall        # to generate .object files
  *   > gcc vertex.o main.o -o vlf.exe      # to generate the executable file
  *   > vlf                                  # to run the program (also vlf.exe)
  */

void
VLF ()
{
  char c;			// Used for read number of lines in the file
  int n = 0;			// Lines counter
  int f = 0;			// Faces counter
  int i;			// Just a counter
  int j = 0;			// Number of vertexes counter
  int k;			// Just a counter
  int m = 0;			// Number of edges counter
  int exist = 0;		// Used for know if a vertex already exist in the vertexes array
  int exists = 0;
  struct vertex aux;		// Auxiliar vertexes
  struct vertex a1;
  struct vertex a2;
  struct vertex a3;
  struct vertex *vertexes;	// Vertexes array
  struct edge *edges;		// Edges array
  struct edge ae1;		// Auxiliar edges
  struct edge ae2;
  struct edge ae3;
  struct face *faces;
  FILE *vlf;			// File pointer to Vertex List Format file
  FILE *elf;			// File pointer to Edges List file
  FILE *flf;			// File pointer to Faces List file
  FILE *raw = fopen ("model.raw", "r");	// File pointer to model.raw file
  while ((c = fgetc (raw)) != EOF)	// Loop to count number of lines in the raw file
    if (c == '\n')		// If theres a line break
      n += 3;			// n+=3, since there are 3 vertexes per line
  printf ("N = %d\n", n);	// Print the number of lines
  rewind (raw);			// Set the file pointer to the beggining of the file
  vertexes = (struct vertex *) malloc (n * sizeof (struct vertex));	// Allocate memory for the vertexes array
  edges = (struct edge *) malloc (n * sizeof (struct edge));	// Allocate memory for the edges array
  faces = (struct face *) malloc ((n / 3) * sizeof (struct face));	// Allocate memory for the faces array
  for (i = 0; i < n; i++)	// Loop to read the vertexes from the file
    {
      fscanf (raw, "%lf %lf %lf ", &aux.x, &aux.y, &aux.z);	// Scan data from the raw file
      aux.hash = aux.x + aux.y + aux.z;	// Set the aux hash value
      if (i > 0)		// If we are reading a value that it's not the first
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
      if (exist != 1)		// If the vertex isn't in the array
	{
	  vertexes[j] = aux;	// Add the vertex to the array
	  vertexes[j].num = j + 1;	// Set the vertex number for the Vertex List Format
	  printf ("Vertex %d\tx=%lf\ty=%lf\tz=%lf\n", vertexes[j].num, vertexes[j].x, vertexes[j].y, vertexes[j].z);	// Print the values
	  j++;			// Increase the vertexes counter
	}
      exist = 0;		// Reset the exist value
    }
  rewind (raw);			// Set again the file pointer to the beggining of the file
  for (i = 0; i < (n / 3); i++)	// Loop to generate the edges list
    {
      fscanf (raw, "%lf %lf %lf %lf %lf %lf %lf %lf %lf ", &a1.x, &a1.y, &a1.z, &a2.x, &a2.y, &a2.z, &a3.x, &a3.y, &a3.z);	/*Read all the line */
      a1.hash = a1.x + a1.y + a1.z;	// Set the a1 hash
      a2.hash = a2.x + a2.y + a2.z;	// Set the a2 hash
      a3.hash = a3.x + a3.y + a3.z;	// Set the a3 hash
      for (k = 0; k < j; k++)	// Compare if the vertex a1 exists in the vertexes array
	{
	  if (a1.hash == vertexes[k].hash)	// If it has the same hash
	    {
	      if (a1.x == vertexes[k].x && a1.y == vertexes[k].y && a1.z == vertexes[k].z)	// Compare if it's the same vertex
		{
		  a1 = vertexes[k];
		  break;
		}
	    }
	}
      for (k = 0; k < j; k++)	// Same as the a1 vertex
	{
	  if (a2.hash == vertexes[k].hash)
	    {
	      if (a2.x == vertexes[k].x && a2.y == vertexes[k].y
		  && a2.z == vertexes[k].z)
		{
		  a2 = vertexes[k];
		  break;
		}
	    }
	}
      for (k = 0; k < j; k++)	// Same as the a1 vertex
	{
	  if (a3.hash == vertexes[k].hash)
	    {
	      if (a3.x == vertexes[k].x && a3.y == vertexes[k].y
		  && a3.z == vertexes[k].z)
		{
		  a3 = vertexes[k];
		  break;
		}
	    }
	}
      if (i > 0)
	{
	  ae1.num = m + 1;
	  ae1.vertex1 = a1;
	  ae1.vertex2 = a2;
	  ae1.hash = a1.hash + a2.hash;
	  for (int l = 0; l < m; l++)
	    {
	      if (ae1.hash == edges[l].hash)
		{
		  if (ae1.vertex1.hash == edges[l].vertex2.hash
		      && ae1.vertex2.hash == edges[l].vertex1.hash)
		    {
		      if (ae1.vertex1.x == edges[l].vertex2.x
			  && ae1.vertex1.y == edges[l].vertex2.y
			  && ae1.vertex1.z == edges[l].vertex2.z
			  && ae1.vertex2.x == edges[l].vertex1.x
			  && ae1.vertex2.y == edges[l].vertex1.y
			  && ae1.vertex2.z == edges[l].vertex1.z)
			{
			  exists = 1;
			  break;
			}
		    }
		  else if (ae1.vertex1.hash == edges[l].vertex1.hash
			   && ae1.vertex2.hash == edges[l].vertex2.hash)
		    {

		      if (ae1.vertex1.x == edges[l].vertex1.x
			  && ae1.vertex1.y == edges[l].vertex1.y
			  && ae1.vertex1.z == edges[l].vertex1.z
			  && ae1.vertex2.x == edges[l].vertex2.x
			  && ae1.vertex2.y == edges[l].vertex2.y
			  && ae1.vertex2.z == edges[l].vertex2.z)
			{
			  exists = 1;
			  break;
			}
		    }
		}
	    }
	  if (exists == 0)
	    {
	      edges[m] = ae1;
	      m++;
	      ae2.num = m + 1;
	      ae2.vertex1 = a2;
	      ae2.vertex2 = a3;
	      ae2.hash = a2.hash + a3.hash;
	    }
	  else
	    {
	      exists = 0;
	      ae2.num = m + 1;
	      ae2.vertex1 = a2;
	      ae2.vertex2 = a3;
	      ae2.hash = a2.hash + a3.hash;
	    }
	  for (int l = 0; l < m; l++)
	    {
	      if (ae2.hash == edges[l].hash)
		{
		  if (ae2.vertex1.hash == edges[l].vertex2.hash
		      && ae2.vertex2.hash == edges[l].vertex1.hash)
		    {
		      if (ae2.vertex1.x == edges[l].vertex2.x
			  && ae2.vertex1.y == edges[l].vertex2.y
			  && ae2.vertex1.z == edges[l].vertex2.z
			  && ae2.vertex2.x == edges[l].vertex1.x
			  && ae2.vertex2.y == edges[l].vertex1.y
			  && ae2.vertex2.z == edges[l].vertex1.z)
			{
			  exists = 1;
			  break;
			}
		    }
		  else if (ae2.vertex1.hash == edges[l].vertex1.hash
			   && ae2.vertex2.hash == edges[l].vertex2.hash)
		    {

		      if (ae2.vertex1.x == edges[l].vertex1.x
			  && ae2.vertex1.y == edges[l].vertex1.y
			  && ae2.vertex1.z == edges[l].vertex1.z
			  && ae2.vertex2.x == edges[l].vertex2.x
			  && ae2.vertex2.y == edges[l].vertex2.y
			  && ae2.vertex2.z == edges[l].vertex2.z)
			{
			  exists = 1;
			  break;
			}
		    }
		}
	    }
	  if (exists == 0)
	    {
	      edges[m] = ae2;
	      m++;
	      ae3.num = m + 1;
	      ae3.vertex1 = a3;
	      ae3.vertex2 = a1;
	      ae3.hash = a3.hash + a1.hash;
	    }
	  else
	    {
	      exists = 0;
	      ae3.num = m + 1;
	      ae3.vertex1 = a3;
	      ae3.vertex2 = a1;
	      ae3.hash = a3.hash + a1.hash;
	    }
	  for (int l = 0; l < m; l++)
	    {
	      if (ae3.hash == edges[l].hash)
		{
		  if (ae3.vertex1.hash == edges[l].vertex2.hash
		      && ae3.vertex2.hash == edges[l].vertex1.hash)
		    {
		      if (ae3.vertex1.x == edges[l].vertex2.x
			  && ae3.vertex1.y == edges[l].vertex2.y
			  && ae3.vertex1.z == edges[l].vertex2.z
			  && ae3.vertex2.x == edges[l].vertex1.x
			  && ae3.vertex2.y == edges[l].vertex1.y
			  && ae3.vertex2.z == edges[l].vertex1.z)
			{
			  exists = 1;
			  break;
			}
		    }
		  else if (ae3.vertex1.hash == edges[l].vertex1.hash
			   && ae3.vertex2.hash == edges[l].vertex2.hash)
		    {

		      if (ae3.vertex1.x == edges[l].vertex1.x
			  && ae3.vertex1.y == edges[l].vertex1.y
			  && ae3.vertex1.z == edges[l].vertex1.z
			  && ae3.vertex2.x == edges[l].vertex2.x
			  && ae3.vertex2.y == edges[l].vertex2.y
			  && ae3.vertex2.z == edges[l].vertex2.z)
			{
			  exists = 1;
			  break;
			}
		    }
		}
	    }
	  if (exists == 0)
	    {
	      edges[m] = ae3;
	      m++;
	    }
	  else
	    {
	      exists = 0;
	    }
	}
      else
	{
	  edges[m].num = m + 1;
	  edges[m].vertex1 = a1;
	  edges[m].vertex2 = a2;
	  edges[m].hash = edges[m].vertex1.hash + edges[m].vertex2.hash;
	  edges[m + 1].num = m + 2;
	  edges[m + 1].vertex1 = a2;
	  edges[m + 1].vertex2 = a3;
	  edges[m + 1].hash =
	    edges[m + 1].vertex1.hash + edges[m + 1].vertex2.hash;
	  edges[m + 2].num = m + 3;
	  edges[m + 2].vertex1 = a3;
	  edges[m + 2].vertex2 = a1;
	  edges[m + 2].hash =
	    edges[m + 2].vertex1.hash + edges[m + 2].vertex2.hash;
	  m += 3;
	}
    }
  rewind (raw);
  for (i = 0; i < (n / 3); i++)	// Start to generate the Faces List
    {
      faces[f].num = f + 1;
      fscanf (raw, "%lf %lf %lf %lf %lf %lf %lf %lf %lf ", &a1.x, &a1.y, &a1.z, &a2.x, &a2.y, &a2.z, &a3.x, &a3.y, &a3.z);	/*Read all the line */
      a1.hash = a1.x + a1.y + a1.z;
      a2.hash = a2.x + a2.y + a2.z;
      a3.hash = a3.x + a3.y + a3.z;
      for (k = 0; k < j; k++)	// Compare if the vertex a1 exists in the vertexes array
	{
	  if (a1.hash == vertexes[k].hash)	// If it has the same hash
	    {
	      if (a1.x == vertexes[k].x && a1.y == vertexes[k].y && a1.z == vertexes[k].z)	// Compare if it's the same vertex
		{
		  a1 = vertexes[k];
		  break;
		}
	    }
	}
      for (k = 0; k < j; k++)	// Same as the a1 vertex
	{
	  if (a2.hash == vertexes[k].hash)
	    {
	      if (a2.x == vertexes[k].x && a2.y == vertexes[k].y
		  && a2.z == vertexes[k].z)
		{
		  a2 = vertexes[k];
		  break;
		}
	    }
	}
      for (k = 0; k < j; k++)	// Same as the a1 vertex
	{
	  if (a3.hash == vertexes[k].hash)
	    {
	      if (a3.x == vertexes[k].x && a3.y == vertexes[k].y
		  && a3.z == vertexes[k].z)
		{
		  a3 = vertexes[k];
		  break;
		}
	    }
	}
      ae1.vertex1 = a1;
      ae1.vertex2 = a2;
      ae1.hash = a1.hash + a2.hash;
      for (k = 0; k < m; k++)
	{
	  if (ae1.hash == edges[k].hash)
	    {
	      if (ae1.vertex1.hash == edges[k].vertex2.hash
		  && ae1.vertex2.hash == edges[k].vertex1.hash)
		{
		  if (ae1.vertex1.x == edges[k].vertex2.x
		      && ae1.vertex1.y == edges[k].vertex2.y
		      && ae1.vertex1.z == edges[k].vertex2.z
		      && ae1.vertex2.x == edges[k].vertex1.x
		      && ae1.vertex2.y == edges[k].vertex1.y
		      && ae1.vertex2.z == edges[k].vertex1.z)
		    {
		      faces[f].edge1 = edges[k];
		      break;
		    }
		}
	      else if (ae1.vertex1.hash == edges[k].vertex1.hash
		       && ae1.vertex2.hash == edges[k].vertex2.hash)
		{

		  if (ae1.vertex1.x == edges[k].vertex1.x
		      && ae1.vertex1.y == edges[k].vertex1.y
		      && ae1.vertex1.z == edges[k].vertex1.z
		      && ae1.vertex2.x == edges[k].vertex2.x
		      && ae1.vertex2.y == edges[k].vertex2.y
		      && ae1.vertex2.z == edges[k].vertex2.z)
		    {
		      faces[f].edge1 = edges[k];
		      break;
		    }
		}
	    }
	}
      ae1.vertex1 = a2;
      ae1.vertex2 = a3;
      ae1.hash = a2.hash + a3.hash;
      for (k = 0; k < m; k++)
	{
	  if (ae1.hash == edges[k].hash)
	    {
	      if (ae1.vertex1.hash == edges[k].vertex2.hash
		  && ae1.vertex2.hash == edges[k].vertex1.hash)
		{
		  if (ae1.vertex1.x == edges[k].vertex2.x
		      && ae1.vertex1.y == edges[k].vertex2.y
		      && ae1.vertex1.z == edges[k].vertex2.z
		      && ae1.vertex2.x == edges[k].vertex1.x
		      && ae1.vertex2.y == edges[k].vertex1.y
		      && ae1.vertex2.z == edges[k].vertex1.z)
		    {
		      faces[f].edge2 = edges[k];
		      break;
		    }
		}
	      else if (ae1.vertex1.hash == edges[k].vertex1.hash
		       && ae1.vertex2.hash == edges[k].vertex2.hash)
		{

		  if (ae1.vertex1.x == edges[k].vertex1.x
		      && ae1.vertex1.y == edges[k].vertex1.y
		      && ae1.vertex1.z == edges[k].vertex1.z
		      && ae1.vertex2.x == edges[k].vertex2.x
		      && ae1.vertex2.y == edges[k].vertex2.y
		      && ae1.vertex2.z == edges[k].vertex2.z)
		    {
		      faces[f].edge2 = edges[k];
		      break;
		    }
		}
	    }
	}
      ae1.vertex1 = a3;
      ae1.vertex2 = a1;
      ae1.hash = a3.hash + a1.hash;
      for (k = 0; k < m; k++)
	{
	  if (ae1.hash == edges[k].hash)
	    {
	      if (ae1.vertex1.hash == edges[k].vertex2.hash
		  && ae1.vertex2.hash == edges[k].vertex1.hash)
		{
		  if (ae1.vertex1.x == edges[k].vertex2.x
		      && ae1.vertex1.y == edges[k].vertex2.y
		      && ae1.vertex1.z == edges[k].vertex2.z
		      && ae1.vertex2.x == edges[k].vertex1.x
		      && ae1.vertex2.y == edges[k].vertex1.y
		      && ae1.vertex2.z == edges[k].vertex1.z)
		    {
		      faces[f].edge3 = edges[k];
		      break;
		    }
		}
	      else if (ae1.vertex1.hash == edges[k].vertex1.hash
		       && ae1.vertex2.hash == edges[k].vertex2.hash)
		{

		  if (ae1.vertex1.x == edges[k].vertex1.x
		      && ae1.vertex1.y == edges[k].vertex1.y
		      && ae1.vertex1.z == edges[k].vertex1.z
		      && ae1.vertex2.x == edges[k].vertex2.x
		      && ae1.vertex2.y == edges[k].vertex2.y
		      && ae1.vertex2.z == edges[k].vertex2.z)
		    {
		      faces[f].edge3 = edges[k];
		      break;
		    }
		}
	    }
	}
      f++;
    }
  for (int x = 0; x < (n / 3); x++)	// loop to print the edges data (to console)
    printf ("Face %d\tEdge %d\tEdge %d Edge %d\n", faces[x].num,
	    faces[x].edge1.num, faces[x].edge2.num, faces[x].edge3.num);
  vlf = fopen ("vlf.txt", "w+");	// Open the Vertex List Format txt file
  elf = fopen ("elf.txt", "w+");	// Open the Edges List txt file
  flf = fopen ("flf.txt", "w+");	// Open the Edges List txt file
  fprintf (vlf, "VLF\n#\tx\t\ty\t\tz\n");	// Print some stuff
  for (i = 0; i < j; i++)	// loop to print the vertexes data to the file
    fprintf (vlf, "%d\t%lf\t%lf\t%lf\n", i + 1, vertexes[i].x, vertexes[i].y,
	     vertexes[i].z);
  fprintf (elf, "ELF\n#\tV1\tV2\n");
  for (int x = 0; x < m; x++)	// loop to print the edges data (to console)
    fprintf (elf, "%d\t%d\t%d\n", edges[x].num,
	     edges[x].vertex1.num, edges[x].vertex2.num);
  fprintf (flf, "FLF\n#\tE1\tE2\tE3\n");	// Print some stuff
  for (i = 0; i < m; i++)	// loop to print the edges data to the file
    fprintf (flf, "%d\t%d\t%d\t%d\n", faces[i].num, faces[i].edge1.num,
	     faces[i].edge2.num, faces[i].edge3.num);
  fclose (raw);			// Close the raw file
  fclose (vlf);			// Close the vlf file
  fclose (elf);			// Close the elf file
  fclose (flf);			// Close the flf file
  free (vertexes);		// Free allocated memory for the vertexes array
  free (edges);			// Free allocated memory for the edges array
  free (faces);			//Free allocated memory for the faces array
}
