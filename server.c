#include <signal.h>
#include <stdio.h>

#include "util.h"
#include "tcp.h"

uint32_t g_num_procs = 0;

// Server main execution loop
void run_server(uint16_t port, char *root_path)
{
    // Sets up signal handler
    init_signalhandler();
    
    // Create a TCP server socket
    int server_socket = create_tcp_socket(port, MAX_CONNECTIONS);

    // Server mainloop
    while (true) {
        int client_socket = accept_tcp_connection(server_socket);
        pid_t child_id = fork();
        if (child_id == -1) {
            die("fork() failed");
        } else if (child_id == 0) {
            close(server_socket);
            process_tcp_connection(client_socket);
            exit(0);
        }

        printf("With child process: %d\n", (int) child_id);
        close(client_socket);
        g_num_procs++;
    }
}

// Sets up signal handler for SIGCHLD, tells syscalls to restart, and blocks all
// signals from firing during event handler
void init_signalhandler(void)
{
    struct sigaction action;
    action.sa_handler = sigchld_handler;
    action.sa_flags = SA_RESTART;
    if (sigfillset(&action.sa_mask) < 0) {
        die("sigfillset() failed");
    }
    if (sigaction(SIGCHLD, &action, NULL) == -1) {
        die("sigaction() failed");
    }
}

// Main signal handler for when SIGCHLD is raised
void sigchld_handler(int sig)
{
    while (g_num_procs > 0) {
        pid_t pid = waitpid((pid_t) -1, NULL, WNOHANG);

        // waitpid() failed case
        if (pid < 0) {
            die("waitpid() failed");
        }
        
        // No child to wait on
        else if (pid == 0) {
            break;
        }
        
        // Cleaning up after child
        else {
            g_num_procs--;
        }
    }
}
