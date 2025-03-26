#include "args.h"

int parseArgs(t_options* options, const char** argv)
{
    int result;

    argv++;
    result = 0;
    while (*argv)
    {
        if (**argv == '-')
            result = parseFlags(options, &argv);
        if (result < 0)
            return -1;
        argv++;
    }
    return 0;
}

int parseFlags(t_options* options, const char*** argv)
{
	int cidx;

    cidx = 0;
    while ((**argv)[++cidx])
    {
        switch ((**argv)[cidx])
        {
        case 'p':
            options->noSort = 1;
            break;
        case 'r':
            options->reverseSort = 1;
            break;
        case 'u':
            options->undefinedOnly = 1;
            break;
        case 'g':
            options->externOnly = 1;
            break;
        case 'a':
            options->printAll = 1;
            break;
        case '?':
            options->help = 1;
            break;
        default:
            return printInvldOption((**argv)[cidx]);
        }
    }
    return 0;
}