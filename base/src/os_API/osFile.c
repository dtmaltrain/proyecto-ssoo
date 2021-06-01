// Import the header file of this module
#include "osFile.h"

osFILE *os_open(char* filename, char* mode)
{
    osFILE* archivo = malloc(sizeof(archivo));
    *archivo = (osFILE){
    .disco = fopen(filename, mode)};
    return archivo;
};

void osfile_destroy(osFILE *osfile)
{
  free(osfile);
}
