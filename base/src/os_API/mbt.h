// Tells the compiler to compile this file once
#pragma once
#include <stdlib.h>

typedef struct entry
{
    int valid;
    int idx_uni;
    unsigned int idx_abs;
    unsigned int n_blocks;
} Entry;

typedef struct mbt
{
    Entry* entries[128];
} MBT;



Entry *entry_init(int valid, int idx_uni, unsigned int idx_abs, unsigned int n_blocks);
void add_entry(MBT *mbt, int valid, int idx_uni, unsigned int idx_abs, unsigned int n_blocks, int pos);
void show_entry(MBT *mbt, int pos);
MBT *mbt_init();
void mbt_destroy(MBT *mbt);