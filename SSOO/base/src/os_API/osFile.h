// Tells the compiler to compile this file once
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "os_API.h"
#include <string.h>


typedef struct osfile {
FILE* disco;
} osFILE;

osFILE *os_open(char* filename, char mode);
// int os_write(osFILE* osfile, void* buffer, int n_bytes);
void osfile_destroy(osFILE *osfile);