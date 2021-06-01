// Tells the compiler to compile this file once
#pragma once

typedef struct directoryEntry {
    int valid;
    unsigned int rel_id;
    char file_name[28];
} DirectoryEntry;

typedef struct directory
{
    DirectoryEntry* entries[64];
} Directory;

Directory *directory_init();
void direc_destroy(Directory *direc);
void add_d_entry(Directory *direc, int valid, unsigned int rel_id, char* file_name, int pos);