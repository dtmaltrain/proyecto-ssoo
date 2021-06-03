// Import used global libraries
#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h> // malloc, calloc, free, etc.
#include <stdint.h>
#include <math.h>
#include <inttypes.h>

// Import the header file of this module
#include "os_API.h"

int mounted = 0;
char *current_disk;
int current_partition;
MBT *mbt;
Directory *direc;
int auxi = 0;
IndexBlock *indexes[64];

void os_mbt()
{
    if (mounted == 0)
    {
        OS_ERROR = DiskNotMounted;
    }
    printf("\nParticiones Validas:\n");
    for (int i = 0; i < 128; i++) // entradas
    {
        // printf("Particion_a %i\n", mbt -> entries[i] -> idx_uni);
        if (mbt->entries[i]->valid == 1) // check bit validez
        {
            printf("Particion %i\n", mbt->entries[i]->idx_uni);
        }
    }
}

// unsigned char *p = (unsigned char *) &i;
// *(p+4) = 9;
void os_reset_mbt()
{
    for (int i = 0; i < 128; i++)
    {
        mbt->entries[i]->valid = 0;
        if (mbt->entries[i]->valid == 1)
        {
            printf("No se reseteó la entrada %d\n", i);
        }
    }
}

void os_delete_partition(int id)
{
    // error si la id >= 128
    if (id >= 128)
    {
        OS_ERROR = IdNotValid;
    }
    for (int i = 0; i < 128; i++)
    {
        if (mbt->entries[i]->idx_uni == id)
        {
            mbt->entries[i]->valid = 0;
        }
    }
}

void sort_array(Entry **array, int n)
{
    Entry *a;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (array[i]->idx_abs > array[j]->idx_abs)
            {
                a = array[i];
                array[i] = array[j];
                array[j] = a;
            }
        }
    }
}

int get_used_space(Entry **occupied)
{
    //MBT* valid_mbt = mbt_init()
    printf("HACEMOS MALLOC\n");
    //unsigned int occupied_size[128];
    int n = 0;
    printf("INICIO FOR DE ENTRIES\n");
    for (int i = 0; i < 128; i++)
    {
        if (mbt->entries[i]->valid == 1)
        {
            occupied[i] = mbt->entries[i];
            n += 1;
        }
    }
    for (int i = 0; i < n; i++)
    {
        printf("idx_abs %u\n", occupied[i]->idx_abs);
        printf("nblocks %u\n", occupied[i]->n_blocks);
    }
    // funcion de sort
    printf("ORDENAR ARRAY\n");
    sort_array(occupied, n);
    printf("ARREGLO CREADO Y ORDENADO\n");
    for (int i = 0; i < n; i++)
    {
        printf("idx_abs %u\n", occupied[i]->idx_abs);
        printf("nblocks %u\n", occupied[i]->n_blocks);
    }
    return n;
}

void os_create_partition(int id, int size)
{
    if (size < 0)
    {
        OS_ERROR = PartitionCreationFailed;
        return;
    }
    int aux = 0;
    int aux2 = 0;
    int id_nuevo;
    for (int i = 0; i < 128; i++)
    {
        if (mbt->entries[i]->idx_uni == id)
        {
            if (mbt->entries[i]->valid == 1)
            {
                OS_ERROR = PartitionCreationFailed;
                aux = 1;
            }
        }
        if (mbt->entries[i]->valid == 0 && aux2 == 0)
        {
            id_nuevo = i;
            aux2 = 1;
        }
    }
    if (aux == 0)
    {

        mbt->entries[id_nuevo]->valid = 1;
        mbt->entries[id_nuevo]->idx_uni = id;
        mbt->entries[id_nuevo]->n_blocks = size;
        printf("SE PUEDE CREAR LA PARTICION %i\n", id);
        Entry *valid_entries[128];
        int n = get_used_space(valid_entries);
        int space = 0;
        int i = 0;
        int pos_ini = 0;
        int pos_fin = 0;
        while (size > space)
        {
            if (i == 0)
            {
                pos_ini = 0;
                pos_fin = valid_entries[i]->idx_abs;
                space = pos_fin - pos_ini;
            }
            else if (i < n)
            {
                pos_ini = valid_entries[i - 1]->idx_abs + valid_entries[i - 1]->n_blocks + 1;
                pos_fin = valid_entries[i]->idx_abs;
                space = pos_fin - pos_ini;
                printf("i: %i\n", i);
                printf("pos_ini_loop: %i\n", pos_ini);
                printf("pos_fin_loop: %i\n", pos_fin);
                printf("space_loop: %i\n", space);
            }
            else if (i == n)
            {
                pos_ini = valid_entries[i - 1]->idx_abs + valid_entries[i - 1]->n_blocks + 1;
                pos_fin = 2097152;
                space = pos_fin - pos_ini;
            }
            else
            {
                break;
            }
            i += 1;
        }
        printf("pos_ini: %i\n", pos_ini);
        printf("space: %i\n", space);
        printf("size: %i\n", size);
        mbt->entries[id_nuevo]->n_blocks = space;
        //codigo para crear
    }
}

void os_ls()
{
    if (mounted == 0)
    {
        OS_ERROR = DiskNotMounted;
    }
    printf("\nArchivos en particion %i:\n", current_partition);
    for (int i = 0; i < 64; i++)
    {
        if (direc->entries[i]->valid == 1)
        {
            printf("%s\n", direc->entries[i]->file_name);
        }
    }
}

int os_exists(char *filename)
{
    if (mounted == 0)
    {
        OS_ERROR = DiskNotMounted;
    }
    // printf("buscando: %s\n", filename);
    for (int i = 0; i < 64; i++)
    {
        if (direc->entries[i]->valid == 1)
        {
            // printf("encontre: %s\n", direc -> entries[i] -> file_name);
            if (strcmp(direc->entries[i]->file_name, filename) == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

int os_rm(char *filename)
{
    // unsigned int direc_id;
    // for (int i = 0; i < 128; i++)
    // {
    //     if (mbt -> entries[i] -> idx_uni == current_partition && mbt -> entries[i] -> valid == 1)
    //     {
    //         direc_id = mbt -> entries[i] -> idx_abs;
    //     }
    // }
    // FILE *drive = fopen(filename, "rb+"); //no se como se abre
    // unsigned char reader;
    for (int i = 0; i < 64; i++)
    {
        if (direc->entries[i]->valid == 1)
        {
            if (strcmp(direc->entries[i]->file_name, filename) == 0)
            {
                direc->entries[i]->valid = 0;
                int n_punteros = ceil((direc->indexes[i]->size + (0.0)) / 2048);
                printf("Numero de punteros %s %d\n", direc->entries[i]->file_name, n_punteros);
                for (int u = 0; u < n_punteros; u++)
                {
                    unsigned int pointed_abs;
                    printf("\ncambiar el bit %u por un 0\n", direc->indexes[i]->pointers[u]);
                    pointed_abs = direc->indexes[i]->pointers[u];

                    int bb;
                    bb = ceil((pointed_abs + (0.0)) / 16384);
                    printf("bloque de bitmap: %d\n", bb);
                    // fseek(drive, 1024 + 2048*direc_id + bb*2048 ,SEEK_SET);

                    // fread(&reader, sizeof(reader), 1, drive);
                    // printf("byte_a_borrar: %d", reader);

                    // fwrite(const void *ptr, size_t size, size_t nmemb, drive)
                }
                // fclose(drive);
                return 1;
            }
        }
    }
    return 0;
}

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
            idx_uni = reader - 128;
        }
        else
        {
            valid = 0;
            idx_uni = reader;
        }

        for (int u = 0; u < 3; u++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            // fread(&reader, sizeof(reader), 1, osfile -> disco);
            idx_abs = idx_abs + (reader << 8 * (2 - u));
        }

        for (int u = 0; u < 4; u++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            // fread(&reader, sizeof(reader), 1, osfile -> disco);
            n_blocks = n_blocks + (reader << 8 * (3 - u));
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
    if (partition > 127)
    {
        OS_ERROR = PartitionNotValid;
    }
    if (auxi == 1)
    {
        if (strcmp(current_disk, diskname) != 0)
        {
            auxi = 2;
            mbt_destroy(mbt);
        }
        direc_destroy(direc);
    }

    current_disk = diskname;
    current_partition = partition;

    populate_mbt(current_disk);

    auxi = 1;

    unsigned int direc_id;
    for (int i = 0; i < 128; i++)
    {
        if (mbt->entries[i]->idx_uni == current_partition && mbt->entries[i]->valid == 1)
        {
            direc_id = mbt->entries[i]->idx_abs;
        }
    }
    // printf("ID ABS %u\n", direc_id);
    direc = directory_init();
    FILE *drive = fopen(current_disk, "rb");
    // printf("Busco en el byte: %u\n\n", 128 + direc_id*2048);
    // printf("el primer seek %d\n", 128*8 + direc_id*(2048));
    fseek(drive, 128 * 8 + direc_id * (2048), SEEK_SET);
    // printf("EL QUE FUNCIONA: the file indicator position is: %ld\n",ftell(drive));
    // printf("FSEEK 1: %i\n",fseek(drive, 128 + direc_id*(2048) , SEEK_SET));
    unsigned char reader;

    for (int i = 0; i < 64; i++)
    {
        int valid;
        unsigned int idx_rel = 0;
        char archivo[28];
        fread(&reader, sizeof(reader), 1, drive);
        valid = reader;
        // printf("valid: %i\n", valid);

        for (int u = 0; u < 3; u++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            // printf("byte: %i\n", reader);
            idx_rel = idx_rel + (reader << 8 * (2 - u));
        }

        for (int u = 0; u < 28; u++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            archivo[u] = reader;
        }

        add_d_entry(direc, valid, idx_rel, archivo, i);
    }
    // fclose(drive);
    for (int i = 0; i < 64; i++)
    {

        unsigned long int pos_idx = direc->entries[i]->rel_id;
        unsigned int n_bloques = 0;
        unsigned long int pos_idxx = 128 * 8 + (direc_id)*2048 + pos_idx * 2048;

        fseek(drive, pos_idxx, SEEK_SET);

        for (int b = 0; b < 5; b++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            n_bloques = n_bloques + (reader << 8 * (4 - b));
        }
        // printf("Tamaño de archivo %s: %u Bloques\n", direc ->entries[i] ->file_name, n_bloques);
        IndexBlock *ind = index_init(n_bloques, pos_idx);

        for (int u = 0; u < 681; u++)
        {
            unsigned int pointer = 0;
            for (int a = 0; a < 3; a++)
            {
                fread(&reader, sizeof(reader), 1, drive);
                pointer = pointer + (reader << 8 * (2 - a));
            }
            add_pointer(ind, pointer, u);
        }
        add_index(direc, ind, i);
    }
    fclose(drive);
    mounted = 1;
}

void os_bitmap(unsigned num)
{
    // error si num > bloques bitmap n_bloques_bitmap

    unsigned int direc_id;
    unsigned int n_blocks;
    for (int i = 0; i < 128; i++)
    {
        if (mbt->entries[i]->idx_uni == current_partition && mbt->entries[i]->valid == 1)
        {
            direc_id = mbt->entries[i]->idx_abs;
            n_blocks = mbt->entries[i]->n_blocks;
        }
    }
    //printf("n bloques %i\n", n_blocks);
    double div = (n_blocks + 0.0) / 16384;
    // printf("%f\n", div);
    int n_bloques_bitmap = ceil(div);
    if (num > n_bloques_bitmap)
    {
        OS_ERROR = NotEnoughBitmap;
    }
    // printf("n blo %d\n", n_bloques_bitmap);
    FILE *drive = fopen(current_disk, "rb");
    unsigned char reader;
    int full = 0;
    int empty = 0;
    if (num != 0)
    {
        fseek(drive, 128 * 8 + direc_id * (2048) + num * 2048, SEEK_SET);
        int iter = 2048;
        if (num == n_bloques_bitmap)
        {
            iter = (n_blocks - (num - 1) * 16384) / 8;
            printf("ITER %d\n", iter);
        }

        for (int i = 0; i < iter; i++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            // printf("\nbyte: %d \n", reader);
            unsigned int bytete = reader;
            for (int j = 0; j < 8; j++)
            {
                if (bytete > pow(2, 7 - j) - 1)
                {
                    bytete = bytete - pow(2, 7 - j);
                    fprintf(stderr, "1");
                    // printf("1");
                    full++;
                }
                else
                {
                    fprintf(stderr, "0");
                    empty++;
                }

                // printf("%u\n", reader  (7-j));
            }
        }

        // fprintf( stderr, "my %s has %d chars\n", "string format", 30);
    }
    else
    {
        fseek(drive, 128 * 8 + direc_id * (2048) + 2048, SEEK_SET);
        int iter = n_blocks / 8;
        printf("ITER %i\n", iter);
        for (int i = 0; i < iter; i++)
        {
            fread(&reader, sizeof(reader), 1, drive);
            // printf("\nbyte: %d \n", reader);
            unsigned int bytete = reader;
            for (int j = 0; j < 8; j++)
            {
                {
                    if (bytete > pow(2, 7 - j) - 1)
                    {
                        bytete = bytete - pow(2, 7 - j);
                        fprintf(stderr, "1");
                        // printf("1");
                        full++;
                    }
                    else
                    {
                        fprintf(stderr, "0");
                        empty++;
                    }

                    // printf("%u\n", reader  (7-j));
                }
            }
        }
    }
    printf("\nCantidad de bloques ocupados %i\n", full);
    printf("\nCantidad de bloques libres %i\n", empty);
    fclose(drive);
}

//char string[8];
//sprintf(string, "%08d", int_to_int(k));
//printf("%08d CORNETAAAAAAA\n", int_to_int(k));
// printf("bit_validez: ")
