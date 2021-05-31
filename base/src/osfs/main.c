#include <stdio.h>
#include "../os_API/os_API.h"

int main(int argc, char **argv)
{
  int num = 1;

  printf("Hello P1!\n");
  char *filename = argv[1];
  printf("%s\n", filename);
  populate_mbt(filename);
  printf("Goodbye P1!\n");

  os_mount(filename, 4);
  //printf("%s\n", current_disk);
  //printf("%i\n", current_partition);
  os_mount(filename, 5);
  //printf("%i\n", current_partition);
}
