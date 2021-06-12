// Tells the compiler to compile this file once
#include "index_block.h"
#pragma once

typedef struct directoryEntry {
    int valid;
    unsigned int rel_id;
    char file_name[28];
} DirectoryEntry;

typedef struct directory
{
    DirectoryEntry* entries[64];
    IndexBlock* indexes[64];
} Directory;

Directory *directory_init();
void direc_destroy(Directory *direc);
void add_d_entry(Directory *direc, int valid, unsigned int rel_id, char* file_name, int pos);
void add_index(Directory *direc, IndexBlock *ind, int pos);