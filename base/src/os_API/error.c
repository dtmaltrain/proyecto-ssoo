#include <stdio.h>
//enum errcode{10, 20, 30, 40, 50, 60, 70};

void os_strerror(int err_code)
{
    if(err_code == 10)
    {
        printf("no funcionó algo");
    }
}