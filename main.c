#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "main.h"
#include "server.h"

int main(int argc, char **argv)
{
    // Check correct arg count
    if (argc < 3) {
        print_usage();
        return 1;
    }

    // Grab command line arguments and validate them
    char *port_err;
    uint16_t port = 0;
    long int port_no = strtol(argv[1], &port_err, BASE_DECIMAL);
    if (port_err == argv[1]) {
        fprintf(stderr, "[error]: conversion error with port number\n");
        return 1;
    } else if (port_no < 0 || port_no > UINT16_MAX) {
        fprintf(stderr, "[error]: port out of specified range of [%d, %d]",
                0, UINT16_MAX);
        return 1;
    } else {
        port = (uint16_t) port_no;
    }
    char *http_root_path = argv[2];
    if (access(http_root_path, F_OK) == -1) {
        fprintf(stderr, "[error]: %s: %s\n", http_root_path, strerror(errno));
        return 1;
    }

    // Execute server
    run_server(argv[1], http_root_path);
}

void print_usage(void)
{
    printf("Usage: myserver <port> <HTTP root dir>\n");
}
