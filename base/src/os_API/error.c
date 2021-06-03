#include <stdio.h>
#include "error.h"
int OS_ERROR = 0;
void os_strerror(int err_code)
{
    if (err_code == 0)
    {
        printf("Finished without errors\n");
    }
    else if (err_code == 1)
    {
        printf("Attempted action requires disk to be mounted.\n");
    }
    else if (err_code == 2)
    {
        printf("Id out of range.\n");
    }
    else if (err_code == 3)
    {
        printf("Bitmap id out of range.\n");
    }
    else if (err_code == 4)
    {
        printf("Partition number out of range.\n");
    }
    else if (err_code == 5)
    {
        printf("Tried to write a file that already exists.\n");
    }
    else if (err_code == 6)
    {
        printf("Partition cannot be created\n");
    }
    else
    {
        printf("It seems like you are toying with our error codes, bad boy\n");
    }
}