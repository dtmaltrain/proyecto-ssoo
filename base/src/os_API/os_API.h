// Tells the compiler to compile this file once
#pragma once
#include "bitmap_block.h"
#include "data_block.h"
#include "directory_block.h"
#include "index_block.h"
#include "mbt.h"
#include "osFile.h"
#include "error.h"

extern char* current_disk;
extern int current_partition;
extern MBT *mbt;
extern Directory *direc;

void populate_mbt(char* filename);
void os_mount(char* diskname, int partition);
int os_exists(char* filename);
void os_ls();
void os_bitmap(unsigned num);