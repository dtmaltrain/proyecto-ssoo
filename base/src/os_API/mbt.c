// Import the header file of this module
#include "mbt.h"
#include <stdio.h>

Entry *entry_init(int valid, int idx_uni, unsigned int idx_abs, unsigned int n_blocks)
{
  Entry *entry = malloc(sizeof(Entry));
  entry -> valid = valid;
  entry -> idx_uni = idx_uni;
  entry -> idx_abs = idx_abs;
  entry -> n_blocks = n_blocks;
  return entry;
}

MBT *mbt_init()
{
    MBT *mbt = malloc(sizeof(MBT));
    *mbt = (MBT){
    };
    return mbt;
};



void add_entry(MBT *mbt, int valid, int idx_uni, unsigned int idx_abs, unsigned int n_blocks, int pos)
{
  Entry *ent = entry_init(valid, idx_uni, idx_abs, n_blocks);
  mbt -> entries[pos] = ent;
  // if (mbt -> entries[pos] -> valid == 1)
  // {
  //   printf("Se agrego la particion %i\n", mbt -> entries[pos] -> idx_uni);
  // }
  
  
}

void show_entry(MBT *mbt, int pos)
{
  if (mbt -> entries[pos] -> valid == 1)
        {
            printf("\n Particion con posicion %i valida:\n", pos);
            printf("Idx_uni: %i\n", mbt -> entries[pos] -> idx_uni);
            printf("Idx_abs: %u\n", mbt -> entries[pos] -> idx_abs);
            printf("n_bloques: %u\n", mbt -> entries[pos] -> n_blocks);
        }
  else{
    printf("\n Particion con posicion %i no es valida\n", pos);
  }
}


void entry_destroy(Entry *entry)
{
  free(entry);
}

void mbt_destroy(MBT *mbt)
{
  for (int i = 0; i < 128; i++)
  {
    entry_destroy(mbt -> entries[i]);
  }
  
  free(mbt);
}

// void mbt_update_in_drive(char* filename, MBT *mbt)
// {
// FILE *f = fopen(filename, "rb+");
// for(int i = 0; i < 128; i++){
//     Entry* entry = mbt -> entries[i];
//     unsigned char reader;
//     unsigned int idx_abs = entry -> idx_abs;
//     unsigned int idx_uni = entry -> idx_uni;
    

// }
// fclose(f);
// return;
// }