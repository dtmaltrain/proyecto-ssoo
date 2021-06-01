// Tells the compiler to compile this file once
#pragma once
#include "bitmap_block.h"
#include "data_block.h"
#include "directory_block.h"
#include "index_block.h"
#include "mbt.h"
#include "osFile.h"

extern char* current_disk;
extern int current_partition;
extern MBT *mbt;

void populate_mbt(char* filename);
void os_mount(char* diskname, int partition);