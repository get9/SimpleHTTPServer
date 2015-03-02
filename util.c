#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>

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

bool file_exists(char *filename)
{
    return (access(filename, F_OK) == 0);
}

// Joins two paths together to make one path
char *join_paths(char *base, char *file)
{
    // + 1 for null byte
    char *newpath = malloc((strlen(base) + strlen(file) + 1) * sizeof(char));
    strncpy(newpath, base, strlen(base));
    // + 1 for null byte
    strncpy(newpath + strlen(base), file, strlen(file) + 1);

    // Remove consecutive slashes
    char *single_slash_path = calloc(strlen(newpath) + 1, sizeof(char));
    int i = 0;
    while (i < strlen(newpath) + 1) {
        if (newpath[i] == '/') {
            single_slash_path[i] = '/';
            while (newpath[i] == '/') {
                i++;
            }
        }
        single_slash_path[i] = newpath[i];
    }
    free(newpath);
    return single_slash_path;
}

// Gets the filename extension for a given file
char *get_extension(char *filename)
{
    if (!file_exists(filename)) {
        return NULL;
    }
    char *ext = strrchr(filename);
    if (ext == NULL || ext == filename) {
        return "";
    } else {
        // ext + 1 to ignore '.'
        return ext + 1;
    }
}
