#include "fileIo.h"

int openFile(const char* path, size_t* fileSize)
{
    int fd;
    struct stat fileStats;
   
    fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        printf("open failed! (%i)\n", errno);
        return -1;
    }
    if (fstat(fd, &fileStats) < 0)
    {
        printf("fstat failed! (%i)\n", errno);
        return -1;
    }
    if (!S_ISREG(fileStats.st_mode))
    {
        printf("invalid file provided\n");
        return -1;
    }
    *fileSize = fileStats.st_size;
    return fd;
}

t_file_range* mapFileRangeToMemory(int fd, size_t offset, size_t size)
{
    t_file_range* mappedRange;

    mappedRange = malloc(sizeof(t_file_range));
    if (!mappedRange)
        return NULL;
    mappedRange->rangeStart = mmap(
        NULL, 
        size, 
        PROT_READ, 
        MAP_PRIVATE,
        fd,
        offset
    );
    if (mappedRange->rangeStart == MAP_FAILED)
    {
        free(mappedRange);
        return NULL;
    }
    mappedRange->rangeSize = size;
    return mappedRange;
}

void unmapFileRange(t_file_range** mappedRange)
{
    munmap((*mappedRange)->rangeStart, (*mappedRange)->rangeSize);
    free(*mappedRange);
    *mappedRange = NULL;
}
