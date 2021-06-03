// Import the header file of this module
#include "osFile.h"

osFILE *os_open(char *filename, char mode)
{
  osFILE *archivo = malloc(sizeof(archivo));
  int exists = os_exists(filename);
  char mode_pointer[1];   // PUNTERO A CHAR INICIALIZADO
  mode_pointer[0] = mode; // SE ASIGNA CHAR AL PUNTERO
  if (strcmp(mode_pointer, "w") == 0)
  { // SE COMPARA char* con char*
    if (exists == 0)
    {
      *archivo = (osFILE){
          .disco = fopen(filename, "w")};
      return archivo;
    }
    else
    {
      printf("No se puede abrir archivo %s en modo escritura, dado que ya existe\n", filename);
      OS_ERROR = FileAlreadyExists;
      return 0;
    }
  }
  else
  {
    *archivo = (osFILE){
        .disco = fopen(filename, "rb")};
    return archivo;
  }
  // else if{
  //   printf("error\n");}
};

// int os_write(osFILE* osfile, void* buffer, int n_bytes){

// }

void osfile_destroy(osFILE *osfile)
{
  free(osfile);
}
