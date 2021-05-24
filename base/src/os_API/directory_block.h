// Tells the compiler to compile this file once
#pragma once

typedef struct directory_block {
    unsigned int bid;
    unsigned int rel_id;
    char type;
} Directory_Block;