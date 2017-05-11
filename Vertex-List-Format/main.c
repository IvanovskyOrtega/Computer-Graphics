#include <stdlib.h>
#include <stdio.h>
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

int
main (void)
{
  VLF ();			// Please check vertex.c to see more about this function
  return 0;
}
