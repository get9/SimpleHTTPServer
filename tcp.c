/*
 * Much of this code was derived from Beej's Guide to Network Programming (as
 * seen here: http://beej.us/guide/bgnet/output/html/multipage/index.html
 * 
 * It is a very useful guide for writing modern networking code in C on a UNIX-
 * style system.
 *
 * All code was written by myself. While it was heavily influence by the above
 * guide, I implemented everything here, looking up functions and their use as
 * I went in order to learn more about networking code in C.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "tcp.h"
#include "util.h"
#include "http_parse.h"

// Creates a TCP socket and returns the socket descriptor
int create_tcp_socket(char *port, int max_conns)
{
    // Get local machine info via getaddrinfo syscall
    struct addrinfo hints;
    struct addrinfo *llinfo;
    memset(&hints, 0, sizeof(hints)); 
    hints = (struct addrinfo) {
        .ai_family = AF_UNSPEC,
        .ai_protocol = IPPROTO_TCP,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE
    };
    int status = getaddrinfo(NULL, port, &hints, &llinfo);
    if (status != 0) {
        die("getaddrinfo() failed");
    }
    
    // Create socket for incoming connections. Must loop through linked list
    // returned by getaddrinfo and try to bind to the first available result
    struct addrinfo *s = NULL;
    int sock = 0;
    for (s = llinfo; s != NULL; s = s->ai_next) {
        // Connect to the socket
        sock = socket(s->ai_family, s->ai_socktype, s->ai_protocol);
        if (sock == -1) {
            die("socket() failed");
        }

        // Set the socket option that gets around "Address already in use"
        // errors
        int tru = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &tru, sizeof(int)) == -1) {
            die("setsockopt() failed");
        }

        // Try to bind to this address; if it doesn't work, go to the next one
        if (bind(sock, s->ai_addr, s->ai_addrlen) == -1) {
            close(sock);
            perror("bind() failed");
            continue;
        }

        // Break out of loop since we got a bound address
        break;
    }

    // Check that we didn't iterate through the entire getaddrinfo linked list
    // and clean up getaddrinfo alloc
    if (s == NULL) {
        die("server could not bind any address");
    }
    freeaddrinfo(llinfo);

    // Set socket to listen for new connections
    if (listen(sock, max_conns) == -1) {
        die("listen() failed");
    }
    
    return sock;
}

// Accepts connections from TCP socket
int accept_tcp_connection(int server_socket)
{
    // Creates place to store client addr info and tries to accept the connection
    struct sockaddr_storage client_addr;
    unsigned int client_addr_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *) &client_addr,
                               &client_addr_len);
    if (client_socket == -1) {
        die("accept() failed");
    }

    // Neat trick to get the correct sockaddr_in struct for IPV4/IPV6
    void *addr_struct = get_addr_struct((struct sockaddr *) &client_addr);
    if (addr_struct == NULL) {
        die("get_addr_struct() failed");
    }

    // Print out connection information
    char addr_str[INET6_ADDRSTRLEN];
    inet_ntop(client_addr.ss_family, addr_struct, addr_str, sizeof(addr_str));
    printf("got connection from: %s\n", addr_str);
    return client_socket;
}

// Handy function to get the correct struct for IPV4 or IPV6 calls
void *get_addr_struct(struct sockaddr *client_addr)
{
    if (client_addr == NULL) {
        return (void *) NULL;
    } else if (client_addr->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) client_addr)->sin_addr);
    } else {
        return &(((struct sockaddr_in6 *) client_addr)->sin6_addr);
    }
}

// Process a TCP connection from a client
void process_tcp_connection(int client_socket)
{
    // Get the request from the client
    char *msgbuf = NULL;
    uint64_t msgbuf_len = 0;
    if (get_request(client_socket, msgbuf, &msgbuf_len) == -1) {
        die("get_request() failed");
    }
}

// Gets request from the client and puts the received bytes in msgbuf with length msgbuf_len
int get_request(int client_socket, char *msgbuf, uint64_t *msgbuf_len)
{
    // Handle input validation, etc
    if (msgbuf_len == NULL) {
        return -1;
    }

    // Alloc memory for new message; initialized to BUFSIZE constant
    msgbuf = malloc(BUFSIZE * sizeof(char));
    *msgbuf_len = BUFSIZE;
    uint64_t content_len = 0;
    int64_t recv_size = 0;

    // Receive bytes
    do {
        recv_size = recv(client_socket, msgbuf + content_len, *msgbuf_len - content_len, 0);
        if (recv_size == -1) {
            free(msgbuf);
            die("recv() failed");
        }
        content_len += (uint64_t) recv_size;

        // Need to resize msgbuf if necessary
        if (recv_size == *msgbuf_len) {
            uint64_t multiplier = 2;
            char *tmpbuf = resize_buf(msgbuf, *msgbuf_len, multiplier);    
            if (tmpbuf == NULL) {
                free(msgbuf);
                die("resize_buf() failed");
            }
            msgbuf = tmpbuf;
            *msgbuf_len *= multiplier;
        }
    } while (recv_size > 0);

    return 0;
}
