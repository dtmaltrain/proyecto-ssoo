// Tells the compiler to compile this file once
#pragma once

typedef struct bitmap_block {
    unsigned int bid;
    unsigned int rel_id;
    char type;
} Bitmap_Block;