// Import the header file of this module
#include "index_block.h"
#include <stdlib.h>

IndexBlock *index_init(unsigned int size, unsigned int pos)
{
    IndexBlock *ind= malloc(sizeof(IndexBlock));
    ind -> size = size;
    ind -> pos = pos;
    return ind;
};

void add_pointer(IndexBlock *ind, unsigned int pointed, int i)
{
    ind -> pointers[i] = pointed;
}

void index_destroy(IndexBlock *ind)
{
    
    free(ind);
    
}