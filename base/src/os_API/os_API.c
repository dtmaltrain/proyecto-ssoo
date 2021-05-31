// Import used global libraries
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
#include <stdint.h>
#include <math.h>
#include <inttypes.h>

// Import the header file of this module
#include "os_API.h"

char *current_disk;
int current_partition;
MBT *mbt;

// void os_mbt()
// {
//     for (int i = 0; i < 128; i++)

//         unsigned long long a = mbt->entries[i];
//     uint8_t *b = (uint8_t *)&a;
//     if (b[0] >= 128)
//     {
//         // b[0] -= 128;
//         printf("PARTICION %d ESTA ACTIVA\n", b[0] - 128);
//     }
// }

// unsigned char *p = (unsigned char *) &i;
// *(p+4) = 9;
// void os_reset_mbt()
// {
//     for (int i = 0; i < 128; i++)
//     {
//         unsigned long long a = mbt->entries[i];
//         uint8_t *b = (uint8_t *)&a;
//         if (b[0] >= 128)
//         {
//             b[0] -= 128;
//             unsigned long long z;
//             memcpy(&z, b, 8);
//             mbt->entries[i] = z;
//         }
//     }
// }

// void os_delete_partition(int id)
// {
//     for (int i = 0; i < 128; i++)
//     {
//         unsigned long long a = mbt->entries[i];
//         uint8_t *b = (uint8_t *)&a;
//         if (b[0] == id + 128)
//         {
//             printf("VOY A BORRAR LA %d\n", id);
//             b[0] = id;
//             unsigned long long z;
//             memcpy(&z, b, 8);
//             mbt->entries[i] = z;
//         }
//     }
// }

unsigned int int_to_int(uint8_t k)
{
    return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_int(k / 2)));
}

void get_binary(uint8_t k)
{
    //char string[8];
    //sprintf(string, "%08d", int_to_int(k));
    //printf("%08d CORNETAAAAAAA\n", int_to_int(k));
    // printf("bit_validez: ")
}

void os_create_partition(int id, int size)
{
    for (int i = 0; i < 128; i++)
    {
        // unsigned char a = mbt->entries[i];
        // uint32_t *r = (uint32_t *)&a;
        // uint64_t *ds = (uint64_t)&a;
        // printf("quehace %i\n", r[1]);
        // printf("%32" PRIx32 "\n", r[1]);
        // printf("%08jx\n", (uintmax_t)r[1]);
        // printf("unsigned char 1 : %u\n", a);
        // break;
        // uint8_t *b = (uint8_t *)&a;
        // printf("partición %d\n", b[0] >> 1);
        // printf("ruta %i %i %i\n", b[1], b[2], b[3]);
        // printf("ruta %i\n", b[3] + (b[2] << 8) + (b[1] << 16));
        // // printf("ruta %i %i %i\n", b[1], b[2], b[3]);
        // // printf("ruta %i\n\n", b[3] + (b[2] << 8) + (b[1] << 16));
        // printf("N_BLOQUES %i %i %i %i\n", b[4], b[5], b[6], b[7]);
        // break;
        // printf("N_BLOQUES %i\n\n", b[7] + (b[6] << 8) + (b[5] << 16) + (b[4] << 24));
        // printf("size %i\n", b[7] + (b[6] << 8) + (b[5] << 16) + (b[4] << 24));
        // printf("size2 %i\n", (b[7] >> 24) + (b[6] >> 16) + (b[5] >> 8) + b[4]);
        // unsigned int num = 2130706432;
        // unsigned int x;
        // unsigned char u[4];

        // u[3] = (num >> 24) & 0xFF;
        // u[2] = (num >> 16) & 0xFF;
        // u[1] = (num >> 8) & 0xFF;
        // u[0] = num & 0xFF;

        // x = *(int *)u;
        // printf("%d\n", x);

        // printf("todo %llu\n", *a);
        // printf("todo2 %i\n", ds);
        //uint32_t bigvar = (b[4] << 24) + (b[5] << 16) + (b[6] << 8) + b[7];
        //uint32_t i32 = b[7] | (b[6] << 8) | (b[5] << 16) | (b[4] << 24);
        //uint32_t x = b[4] | (b[5] << 8) | (b[6] << 16) | (b[7] << 24);
        //uint32_t t = (b[7]<< 0) | (b[6] << 8) | (b[5] << 16) | (b[4] << 24);
        //int i = (int)t;
        //printf("size2 %u\n", t);
        //printf("%" PRIu32 "\n",t);
        //printf("ENTERO: %i\n",i);
    }
}

// void SwapBytes(void *pv, size_t n)
// {
//     assert(n > 0);

//     char *p = pv;
//     size_t lo, hi;
//     for (lo = 0, hi = n - 1; hi > lo; lo++, hi--)
//     {
//         char tmp = p[lo];
//         p[lo] = p[hi];
//         p[hi] = tmp;
//     }
// }

// https://stackoverflow.com/questions/8556927/why-does-fread-mess-with-my-byte-order

void populate_mbt(char *filename)
{
    mbt = mbt_init();
    FILE *drive = fopen(filename, "rb");

    for (int i = 0; i < 128; i++)
    {
        unsigned char reader;
        int valid;
        int idx_uni = 0;
        unsigned int idx_abs = 0;
        unsigned int n_blocks = 0;
        fread(&reader, sizeof(reader), 1, drive);

        if (reader > 127)
        {   
            valid = 1;
            idx_uni = reader - 128 ;
        }else{
            valid = 0;
            idx_uni = reader;
        }

        for (int u = 0; u < 3; u++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            idx_abs = idx_abs + (reader << 8*(2-u));
        }

        for (int u = 0; u < 4; u++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            n_blocks = n_blocks + (reader << 8*(3-u));
        }
        // if (valid == 1)
        // {
        //     printf("Idx_uni: %i\n", idx_uni);
        //     printf("Idx_abs: %u\n", idx_abs);
        //     printf("n_bloques: %u\n", n_blocks);
        // }
        
        
        add_entry(mbt, valid, idx_uni, idx_abs, n_blocks, i);
    }
    show_entry(mbt, 0);
    show_entry(mbt, 3);
    // printf("La 0: %i\n", mbt -> entries[0] -> valid);
    fclose(drive);

    // os_mbt(mbt);
    //unsigned long long* a = mbt->entries[13];
    //printf("%llu\n", &a);
    //printf("%lu\n", sizeof(a));
    // int bitStatus = (12 >> 0) & 1;
    // printf("The bit is set to %d\n", bitStatus);
    //uint8_t b[8];
    //memcpy(b, &a, 8);
    //uint8_t target_byte = b[0];
    // uint8_t* b = (uint8_t*)&a;
    // uint8_t target_byte = b[0];
    // printf("%d\n", target_byte);
    // get_binary(b[0]);
    // os_reset_mbt();
    // os_delete_partition(123);
    //os_mbt();
    // os_create_partition(1, 4);
    //i & (1 << N);
    // int ferz = func(target_byte, 0);

    mbt_destroy(mbt);
}

void os_mount(char *diskname, int partition)
{
    current_disk = diskname;
    current_partition = partition;
}

// https://stackoverflow.com/questions/14564813/how-to-convert-an-integer-to-a-character-array-using-c

char *toArray(int number)
{
    int n = log10(number) + 1;
    int i;
    char *numberArray = calloc(n, sizeof(char));
    for (i = n - 1; i >= 0; --i, number /= 10)
    {
        numberArray[i] = (number % 10) + '0';
    }
    return numberArray;
}

// https://stackoverflow.com/questions/5488377/converting-an-integer-to-binary-in-c
