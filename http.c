#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

#include "http.h"

// Parses the request and fills the struct http_req_t
int parse_request(char *http_request, uint64_t req_len, struct http_req_t *req)
{
    char **lines = splitlines(http_request, req_len);
    parse_method_line(lines[0], req);

    // Cleanup from parse
    int i = 0;
    while (lines[i] != NULL) {
        free(lines[i]);
    }
    free(lines);
    return 0;
}

// Splits req into its associated lines of content
char **splitlines(char *req, uint64_t len)
{
    // + 1 for sentinel value at the end
    uint64_t numlines = count_lines(req, len);
    char **lines = malloc((numlines + 1) * sizeof(char *));

    // Split into lines
    uint64_t i = 0;
    char *line = strtok(req, "\r\n");
    while (line != NULL) {
        // + 1 for null
        lines[i] = calloc(sizeof(char), (strlen(line) + 1));
        strncpy(lines[i], line, strlen(line));
        line = strtok(NULL, "\r\n");
        i++;
    }

    // Add sentinel value
    lines[i] = NULL;
    return lines;
}

// Counts the number of lines in req
uint64_t count_lines(char *req, uint64_t len)
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
void parse_method_line(char *method_line, struct http_req_t *req)
{
    // Grab method name
    char *tmp = strtok(method_line, " ");
    memset(req->method, 0, strlen(req->method));
    strncpy(req->method, tmp, strlen(req->method));
    // Grab file
    tmp = strtok(NULL, " ");
    req->file = strdup(tmp);
    // Grap version info
    tmp = strtok(NULL, " ");
    req->vers = strdup(tmp);
}

// Determine the response code based on whether the file exists
char *get_response_code(char *filename)
{
    return (file_exists(filename) ? "200 OK" : "404 Not Found");
}

// Determine content type associated with this file
// Only handle 4 types:
//     * text/plain (.txt)
//     * text/html (.html)
//     * image/jpeg (.jpg, .jpeg)
//     * image/gif (.gif)
char *get_content_type(char *filename)
{
    char *ext = get_extension(filename);
    if (ext == NULL) {
        return NULL;
    }
    if (strncmp(ext, "html", strlen("html")) == 0 || 
        strncmp(ext, "html", strlen("htm")) == 0) {
        return "text/html";
    } else if (strncmp(ext, "jpeg", strlen("jpeg")) == 0 ||
               strncmp(ext, "jpg", strlen("jpg")) == 0) {
        return "image/jpeg";
    } else if (strncmp(ext, "gif", strlen("gif")) == 0) {
        return "image/gif";
    } else {
        return "text/plain";
    }
}

// Gets the actual content and stores a pointer to it in the structure
char *get_content(char *filename)
{
    if (!file_exists(filename)) {
        return NULL;
    }

    // Determine file size first
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long int fsize = ftell(f);
    rewind(f);

    // Read contents to buffer
    char *buf = malloc((fsize + 1) * sizeof(char));
    fread(buf, fsize, sizeof(char), f);
    if (ferror(f)) {
        free(buf);
        return NULL;
    }
    fclose(f);
    buf[fsize] = '\0';

    return buf;
}

// Copy over the version information
char *get_vers(char *vers)
{
    char *newvers = malloc((strlen(vers) + 1) * sizeof(char));
    strncpy(newvers, vers, strlen(vers));
    newvers[strlen(vers)] = '\0';
    return newvers;
}

// Cleanup functions
void destroy_http_req_t(struct http_req_t *req)
{
    free(req->file);
    free(req->vers);
}

void destroy_http_resp_t(struct http_resp_t *resp)
{
    free(resp->rc);
    free(resp->content_type);
    free(resp->content);
    free(resp->vers);
}
