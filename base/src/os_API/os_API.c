// Import used global libraries
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.

// Import the header file of this module
#include "os_API.h"

void populate_mbt(char* filename){
    MBT *mbt = mbt_init();
    FILE *drive = fopen(filename, "r");
    fread(&mbt->entries, 8, 128, drive);
    unsigned int* a = mbt->entries[127];
    printf("%p\n", (void *)a);
    mbt_destroy(mbt);
}

