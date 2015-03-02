#include <string.h>

#include "http_parse.h"

// Parses the request and fills the struct http_req_t
int parse_request(char *http_request, uint64_t req_len, struct http_req_t *req)
{
    char **lines = splitlines(http_request, req_len);
    parse_method_line(lines[0], req);
    return 0;
}

// Splits req into its associated lines of content
char **splitlines(char *req, uint64_t len);
{
    // + 1 for sentinel value at the end
    uint64_t numlines = count_lines(req, len);
    char **lines = malloc((numlines + 1) * sizeof(char *));

    // Split into lines
    uint64_t i = 0;
    char *line = strtok(req, "\n");
    while (line != NULL) {
        line = strtok(NULL, "\n");
        lines[i] = strdup(line);
        i++;
    }

    // Add sentinel value
    lines[i] = NULL;
    return lines;
}

// Counts the number of lines in req
uint64_t count_lines(char *req, uint64_t len);
{
    uint64_t count = 0;
    for (uint64_t i = 0; i < len; ++i) {
        if (req[i] == '\n') {
            count++;
        }
    }
    return count;
}

// Parses the method line from method_line and fills the struct http_req_t
void parse_method_line(char *method_line, struct http_req_t *req);
{
    char *tmp = strtok(method_line, " ");
    // Grab method name
    tmp = strtok(NULL, " ");
    strncpy(req->method, tmp, strlen(req->method));
    // Grab file
    tmp = strtok(NULL, " ");
    req->file = strdup(tmp);
    // Grap version info
    tmp = strtok(NULL, " ");
    req->vers = strdup(vers);
}
