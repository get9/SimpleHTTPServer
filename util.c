#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void die(char *errmsg)
{
    perror(errmsg);
    exit(1);
}
