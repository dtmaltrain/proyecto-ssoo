// Import the header file of this module
#include "mbt.h"

MBT *mbt_init()
{
    MBT *mbt = malloc(sizeof(MBT));
    *mbt = (MBT){
    };
    return mbt;
};

void mbt_destroy(MBT *mbt)
{
  free(mbt);
}