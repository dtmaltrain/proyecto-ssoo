#include <stdio.h>
#include "../os_API/os_API.h"


int main(int argc, char **argv)
{
  printf("Hello P1!\n");
  char *filename = argv[1];
  printf("%s\n", filename);
  populate_mbt(filename);
  printf("Goodbye P1!\n");
}
