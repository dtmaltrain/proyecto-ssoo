#include <stdio.h>
#include "../os_API/os_API.h"
#include <math.h>

int main(int argc, char **argv)
{

  printf("Hello P1!\n");
  char *filename = argv[1];

  os_mount(filename, 2);

  os_mbt();
  // os_rm("nene.txt");

  mbt_update_in_drive(filename, mbt);

  // os_mount(filename, 2);

  // os_mbt();
  // unsigned int num = 0;

  // os_bitmap(num);

  os_ls();
  //OLA GRUPOOOOO; oal

  // for (int i = 0; i < 64; i++)
  // {
  //   if (direc -> entries[i] -> valid == 1)
  //     {
  //       int n_punteros = ceil((direc -> indexes[i] -> size + (0.0))/2048);
  //       // printf("Numero de punteros %s %d\n", direc ->entries[i]-> file_name, n_punteros);
  //       // printf("puntero  0 %u\n",direc -> indexes[i] -> pointers[2]);
  //     }
  // }
  //escribir directorio
  direc_destroy(direc);

  mbt_destroy(mbt);

  os_strerror(OS_ERROR);

  //char b = 'r';
  //osFILE* ola = os_open(filename, b);

  //printf("%s\n", current_disk);
  //printf("%i\n", current_partition);
  //printf("%i\n", current_partition);

  printf("\nGoodbye P1!\n");
}
