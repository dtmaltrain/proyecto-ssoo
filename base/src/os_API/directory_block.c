// Import the header file of this module
#include "directory_block.h"
#include <stdio.h>
#include <stdlib.h>

DirectoryEntry *d_entry_init(int valid, unsigned int rel_id, char* file_name)
{
  DirectoryEntry *entry = malloc(sizeof(DirectoryEntry));
  entry -> valid = valid;
  entry -> rel_id = rel_id;
  for (int i = 0; i < 28; i++)
  {
    entry -> file_name[i] = file_name[i];
  }
  return entry;
}

Directory *directory_init()
{
    Directory *direc = malloc(sizeof(Directory));
    *direc = (Directory){
    };
    return direc;
};

void add_d_entry(Directory *direc, int valid, unsigned int rel_id, char* file_name, int pos)
{
  DirectoryEntry *ent= d_entry_init(valid, rel_id, file_name);
  direc -> entries[pos] = ent;

}

void d_entry_destroy(DirectoryEntry *entry)
{
  free(entry);
}

void direc_destroy(Directory *direc)
{
  for (int i = 0; i < 64; i++)
  {
    d_entry_destroy(direc -> entries[i]);
  }
  
  free(direc);
}