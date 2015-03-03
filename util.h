#include <inttypes.h>
#include <stdbool.h>

#pragma once

_Noreturn void die(char *);
char *resize_buf(char *buf, uint64_t oldlen, uint64_t mul_factor);
bool file_exists(char *filename);
char *join_paths(char *base, char *file);
char *get_extension(char *filename);
