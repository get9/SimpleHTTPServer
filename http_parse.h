#include <inttypes.h>

// Holds informtion about a request (file, method used, etc)
struct http_req_t {
    char *file;
    char *vers;
    char method[10];
    // Could have other header info, but don't need them for now
};

int parse_request(char *http_request, uint64_t req_len, struct http_req_t *req);
char **splitlines(char *req, uint64_t len);
uint64_t count_lines(char *req, uint64_t len);
void parse_method_line(char *method_line, struct http_req_t *req);
