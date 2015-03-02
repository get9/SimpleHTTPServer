#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "util.h"

void die(char *errmsg)
{
    perror(errmsg);
    exit(1);
}

char *resize_buf(char *buf, uint64_t oldlen, uint64_t mul_factor)
{
    if (buf == NULL) {
        return NULL;
    }
    char *tmpbuf = realloc(buf, oldlen * mul_factor);
    return (tmpbuf == buf ? NULL : tmpbuf);
}
