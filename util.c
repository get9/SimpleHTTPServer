#include <stdio.h>

void die(char *errmsg)
{
    perror(errmsg);
    exit(1);
}
