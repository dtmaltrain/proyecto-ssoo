#ifndef ERROR_H
#define ERROR_H

int OS_ERROR;
typedef enum
{
    DiskNotMounted = 1,
    IdNotValid = 2,
    NotEnoughBitmap = 3,
    PartitionNotValid = 4,
    FileAlreadyExists = 5,
    PartitionCreationFailed = 6
} errcodes;

#endif
void os_strerror(int err_code);