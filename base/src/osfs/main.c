#include <stdio.h>
#include "../os_API/os_API.h"

int main(int argc, char **argv)
{
  // int num = 1;

  printf("Hello P1!\n");
  char *filename = argv[1];
  os_mount(filename, 4);
  
  printf("%s\n", filename);
  printf("\nGoodbye P1!\n");

  
  //printf("%s\n", current_disk);
  //printf("%i\n", current_partition);
  //printf("%i\n", current_partition);
}
