// Tells the compiler to compile this file once
#pragma once

typedef struct indexblock {
    unsigned int size;
    unsigned int pos;
    unsigned int pointers[681];
} IndexBlock;

IndexBlock *index_init(unsigned int size, unsigned int pos);
void add_pointer(IndexBlock *ind, unsigned int pointed, int i);
void index_destroy(IndexBlock *ind);