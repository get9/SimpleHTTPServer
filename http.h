#include <inttypes.h>

#define CONTENT_LEN 4096

// Holds informtion about a request (file, method used, etc)
struct http_req_t {
    char *file;
    char *vers;
    char method[10];
    // Could have other header info, but don't need them for now
};

// Holds information abut a response code, content type, length, pointer to content
struct http_resp_t {
    char *rc;
    char *content_type;
    uint64_t content_len;
    char *content;
    char *vers;
};

int parse_request(char *http_request, uint64_t req_len, struct http_req_t *req);
char **splitlines(char *req, uint64_t len);
uint64_t count_lines(char *req, uint64_t len);
void parse_method_line(char *method_line, struct http_req_t *req);
char *get_response_code(char *filename);
char *get_content_type(char *filename);
char *get_content(char *filename);
char *get_vers(char *vers);
void destroy_http_req_t(struct http_req_t *req);
void destroy_http_resp_t(struct http_resp_t *req);
