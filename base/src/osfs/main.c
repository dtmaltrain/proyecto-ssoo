#include <stdio.h>
#include "../os_API/os_API.h"

int main(int argc, char **argv)
{
  // int num = 1;

  printf("Hello P1!\n");
  char *filename = argv[1];
  os_mount(filename, 2);
  os_ls();
  // os_rm("nene.txt");
  // os_ls();

  os_mount(filename, 3);
  os_ls();

  direc_destroy(direc);
  mbt_destroy(mbt);
  //printf("%s\n", current_disk);
  //printf("%i\n", current_partition);
  //printf("%i\n", current_partition);
  printf("\nGoodbye P1!\n");
}
