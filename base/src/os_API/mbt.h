// Tells the compiler to compile this file once
#pragma once
#include <stdlib.h>

typedef struct mbt {
    unsigned int bid;
    unsigned int rel_id;
    int n_entry;
    int entry_size;
    unsigned long long* entries[128];
} MBT;


MBT *mbt_init();
void mbt_destroy(MBT *mbt);