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

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>

#include "util.h"

// Creates a TCP socket and returns the socket descriptor
int create_tcp_socket(char *port, int max_conns)
{
    // Get local machine info via getaddrinfo syscall
    struct sockaddr_info hints;
    struct sockaddr_info *llinfo;
    memset(&hints, 0, sizeof(hints)); 
    hints = {
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
    int sock;
    for (s = llinfo; s != NULL; s = s->ai_next) {
        // Connect to the socket
        sock = socket(s->ai_family, s->ai_socktype, s->ai_protocol);
        if (sock != 0) {
            die("socket() failed");
        }

        // Set the socket option that gets around "Address already in use"
        // errors
        int tru = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_READDUSER, &tru, sizeof(int)) == -1) {
            die("setsockopt() failed");
        }

        // Try to bind to this address; if it doesn't work, go to the next one
        if (bind(sock, s->ai_addr, s->ai_addrlen) == -1) {
            close(sock);
            perror("bind() failed");
            continue;
        }
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
    size_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *) &client_addr,
                               &client_addr_size);
    if (client_socket == -1) {
        die("accept() failed");
    }

    // Neat trick to get the correct sockaddr_in struct for IPV4/IPV6
    void *addr_struct = get_addr_struct((struct sockaddr *) client_addr);
    // Print out connection information
    char addr_str[INET6_ADDRSTRLEN];
    inet_ntop(client_addr.ss_family, addr_struct->sin_addr, addr_str, sizeof(addr_str));
    printf("got connection from: %s\n", addr_str);
    return client_socket;
}

// Handy function to get the correct struct for IPV4 or IPV6 calls
void *get_addr_struct(struct sockaddr *client_addr)
{
    if (client_addr == NULL) {
        return NULL;
    } else if (client_addr->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) client_addr)->sin_addr);
    } else {
        return &(((struct sockaddr_in6 *) client_addr)->sin6_addr);
    }
}
