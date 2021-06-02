#include <stdio.h>
#include "../os_API/os_API.h"

int main(int argc, char **argv)
{
  

  printf("Hello P1!\n");
  char *filename = argv[1];
  // os_rm("nene.txt");
  // os_ls();

  os_mount(filename, 2);

  unsigned int num = 0;

  os_bitmap(num);

  // os_ls();
  //OLA GRUPOOOOO; oal

  // for (int i = 0; i < 64; i++)
  // {
  //   if (direc -> entries[i] -> valid == 1)
  //     {
  //       printf(<"%u\n",indexes[i] -> size);
  //         // index_destroy(indexes[i]);
  //     } 
  // }
  direc_destroy(direc);
  //mbt_update_in_drive(filename, mbt);
  mbt_destroy(mbt);
  //char b = 'r';
  //osFILE* ola = os_open(filename, b);

  //printf("%s\n", current_disk);
  //printf("%i\n", current_partition);
  //printf("%i\n", current_partition);

  printf("\nGoodbye P1!\n");
}
