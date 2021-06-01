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
Directory *direc;

void os_mbt()
{   
    printf("\nParticiones Validas:\n");
    for (int i = 0; i < 128; i++)
    {
        if (mbt -> entries[i] -> valid == 1)
        {
            printf("Particion %i\n", mbt -> entries[i] -> idx_uni);
        }
        
    } 
}

// unsigned char *p = (unsigned char *) &i;
// *(p+4) = 9;
void os_reset_mbt()
{
    for (int i = 0; i < 128; i++)
    {
        mbt -> entries[i] -> valid = 0;
        if (mbt -> entries[i] -> valid == 1)
        {
            
        }
    }
}

void os_delete_partition(int id)
{
    for (int i = 0; i < 128; i++)
    {
        if (mbt -> entries[i] -> idx_uni == id)
        {
            mbt -> entries[i] -> valid = 0;
        }
        
    }
}

void sort_array(Entry** array, int n){
    Entry* a;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j){
            if (array[i]-> idx_abs > array[j]-> idx_abs){
                a =  array[i];
                array[i] = array[j];
                array[j] = a;
            }
        }
    }
}

int get_used_space(Entry** occupied)
{
    //MBT* valid_mbt = mbt_init()
    printf("HACEMOS MALLOC\n");
    //unsigned int occupied_size[128];
    int n = 0;
    printf("INICIO FOR DE ENTRIES\n");
    for (int i = 0; i < 128; i++)
    {
        if (mbt -> entries[i] -> valid == 1)
        {   
            occupied[i] = mbt -> entries[i]; 
            n +=1;
        }
    }
    for (int i = 0; i < n; i++)
    {
        printf("idx_abs %u\n", occupied[i]-> idx_abs);
        printf("nblocks %u\n", occupied[i]-> n_blocks);
    }
    // funcion de sort
    printf("ORDENAR ARRAY\n");
    sort_array(occupied, n);
    printf("ARREGLO CREADO Y ORDENADO\n");
    for (int i = 0; i < n; i++)
    {
        printf("idx_abs %u\n", occupied[i]-> idx_abs);
        printf("nblocks %u\n", occupied[i]-> n_blocks);
    }
    return n;

}

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
    int aux = 0;
    int aux2 = 0;
    int id_nuevo;
    for (int i = 0; i < 128; i++)
    {
        if (mbt -> entries[i] -> idx_uni == id)
        {
            if(mbt -> entries[i] -> valid == 1)
            {
                printf("NO SE PUEDE CREAR LA PARTICION %i\n", id); 
                aux = 1;               
            }
            
        }
        if (mbt -> entries[i] -> valid == 0 && aux2 == 0)
        {
            id_nuevo = i;
            aux2 = 1;
        }
        
    }
    if (aux == 0)
    {
        
        mbt -> entries[id_nuevo] -> valid = 1;
        mbt -> entries[id_nuevo] -> idx_uni = id;
        mbt -> entries[id_nuevo] -> n_blocks = size;
        printf("SE PUEDE CREAR LA PARTICION %i\n", id);
        Entry* valid_entries[128];
        int n = get_used_space(valid_entries);
        int space = 0;
        int i = 0;
        int pos_ini = 0;
        int pos_fin = 0;
        while(size > space){
            if (i == 0){
                pos_ini = 0;
                pos_fin = valid_entries[i] -> idx_abs;
                space = pos_fin - pos_ini;
            }
            else if (i < n){
                pos_ini = valid_entries[i-1] -> idx_abs + valid_entries[i-1] -> n_blocks + 1;
                pos_fin = valid_entries[i] -> idx_abs;
                space = pos_fin - pos_ini;
                printf("i: %i\n", i);
                printf("pos_ini_loop: %i\n", pos_ini);
                printf("pos_fin_loop: %i\n", pos_fin);
                printf("space_loop: %i\n", space);
            }
             else if(i == n){
                pos_ini = valid_entries[i-1] -> idx_abs + valid_entries[i-1] -> n_blocks + 1;
                pos_fin = 2097152;
                space = pos_fin - pos_ini;
            }
            else{
                break;
            }
            i +=1;
        }
        printf("pos_ini: %i\n", pos_ini);
        printf("space: %i\n", space);
        printf("size: %i\n", size);
        mbt -> entries[id_nuevo] -> n_blocks = space;
        //codigo para crear   
    }
}

void os_ls(){


    for (int i = 0; i < 64; i++)
    {   
        if (direc -> entries[i] -> valid == 1)
        {
            printf("%s\n", direc -> entries[i] -> file_name);
        }
    }
    
    
}


int os_exists(char* filename)
{
    // printf("buscando: %s\n", filename);
    for (int i = 0; i < 64; i++)
    {   
        if (direc -> entries[i] -> valid == 1)
        {
            // printf("encontre: %s\n", direc -> entries[i] -> file_name);
            if (strcmp(direc -> entries[i] -> file_name,  filename) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
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
    // osFILE* osfile = os_open(filename, "r");
    // osfile -> disco;
    FILE *drive = fopen(filename, "rb");

    for (int i = 0; i < 128; i++)
    {
        unsigned char reader;
        int valid;
        int idx_uni = 0;
        unsigned int idx_abs = 0;
        unsigned int n_blocks = 0;
        fread(&reader, sizeof(reader), 1, drive);
        // fread(&reader, sizeof(reader), 1, osfile -> disco);

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
            // fread(&reader, sizeof(reader), 1, osfile -> disco);
            idx_abs = idx_abs + (reader << 8*(2-u));
        }

        for (int u = 0; u < 4; u++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            // fread(&reader, sizeof(reader), 1, osfile -> disco);
            n_blocks = n_blocks + (reader << 8*(3-u));
        }
        
        add_entry(mbt, valid, idx_uni, idx_abs, n_blocks, i);
    }

    fclose(drive);
    // fclose(osfile -> disco);
    
    // os_create_partition(2, 23312);
    // os_create_partition(5, 23312);
    // os_mbt();
    
    
   
    // osfile_destroy(osfile);

}

void os_mount(char *diskname, int partition)
{
    current_disk = diskname;
    current_partition = partition;
    populate_mbt(current_disk);

    unsigned int direc_id;
    for (int i = 0; i < 128; i++)
    {
        if (mbt -> entries[i] -> idx_uni == current_partition && mbt -> entries[i] -> valid == 1)
        {
            direc_id = mbt -> entries[i] -> idx_abs;
        }
    }
    printf("ID ABS %u\n", direc_id);
    direc = directory_init();
    FILE *drive = fopen(current_disk, "rb");
    fseek(drive, 128 + direc_id*(2048), SEEK_SET);
    
    for (int i = 0; i < 64; i++)
    {
        unsigned char reader;
        int valid;
        unsigned int idx_rel = 0;
        char archivo[28];
        fread(&reader, sizeof(reader), 1, drive);
        valid = reader;
        

        for (int u = 0; u < 3; u++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            idx_rel = idx_rel + (reader << 8*(2-u));
        }

        for (int u = 0; u < 28; u++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            archivo[u] = reader;
        }

        // if (valid == 1)
        // {
        //     printf("%s\n", archivo);
        // }
        
            
        add_d_entry(direc, valid, idx_rel, archivo, i);
    }
    
    fclose(drive);
    
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
