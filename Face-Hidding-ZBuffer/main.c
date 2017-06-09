#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include"bresenham.h"
#include"vertex.h"

int
main (int argc, char *argv[])
{
  double alfa = atof (argv[1]);
  double beta = atof (argv[2]);
  double gamma = atof (argv[3]);
  double sfX = atof (argv[4]);
  double sfY = atof (argv[5]);
  double tX = atof (argv[6]);
  double tY = atof (argv[7]);
  VLF (alfa, beta, gamma, sfX, sfY, tX, tY, argv[8]);	//Functions and methods in bresenham.c
  return 0;
}
