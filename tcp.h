#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_CONNECTIONS 5

int create_tcp_socket(char *port, int num_conn);
int accept_tcp_connection(int server_socket);
void *get_addr_struct(struct sockaddr *client_addr);
void process_tcp_connection(int client_socket);
