#include <inttypes.h>

_Noreturn void die(char *);
char *resize_buf(char *buf, uint64_t oldlen, uint64_t mul_factor);
