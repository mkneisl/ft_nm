#ifndef FILEIO_H
#define FILEIO_H
#include <fcntl.h>
#include <stddef.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "libft.h"

typedef struct s_file_range
{
    void* rangeStart;
    size_t rangeSize;
    size_t offset;
}t_file_range;

int             openFile(const char* path, size_t* fileSize);
t_file_range*   mapFileRangeToMemory(int fd, size_t offset, size_t size);
void            unmapFileRange(t_file_range** mappedRange);
#endif
