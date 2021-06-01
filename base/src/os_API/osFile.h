// Tells the compiler to compile this file once
#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct osfile {
FILE* disco;
} osFILE;

osFILE *os_open(char* filename, char* mode);
void osfile_destroy(osFILE *osfile);