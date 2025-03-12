#include <unistd.h>


int main(int argc, char** argv)
{
    char* fileName;
    
    if (argc < 2)
        fileName = "a.out";
    else
        fileName = argv[1];

    return 0;
}
