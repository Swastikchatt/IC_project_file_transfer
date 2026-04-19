/* Transport API (see transport.h) plus standard library headers for I/O and memset.

 * POSIX/Linux only — standard BSD sockets (no Windows/Winsock). */

#include "transport.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/* Create a single IPv4 TCP socket; returns -1 on failure (caller checks). */

static int create_tcp_socket(void) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket(AF_INET, SOCK_STREAM)");
    }
    return sock;
}

/* Outbound connection: connect to host:port and return the socket as an int handle (or -1 on error). */

static int run_client(const char* host, int port) {
    int sock = create_tcp_socket();
    if (sock == -1) {
        return -1;
    }

    /* Fill in IPv4 address + port (network byte order) for connect(). inet_addr expects dotted IPv4. */

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);
    addr.sin_addr.s_addr = inet_addr(host);

    /* TCP connect to the server; on failure print error, close socket, return -1. */

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "connect to %s:%d: ", host, port);
        perror(NULL);
        close(sock);
        return -1;
    }

    printf("Connected to %s:%d\n", host, port);

    /* Caller owns the socket file descriptor. */

    return sock;
}

/* Listen on port, accept exactly one client, return the data socket (or -1 on error). */

static int run_server(int port) {
    int listen_sock = create_tcp_socket();
    if (listen_sock == -1) {
        return -1;
    }

    /* Bind to INADDR_ANY so we listen on all interfaces; port in network byte order. */

    struct sockaddr_in bind_addr;
    memset(&bind_addr, 0, sizeof(bind_addr));
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons((unsigned short)port);
    bind_addr.sin_addr.s_addr = INADDR_ANY;

    /* Attach the listening socket to the chosen local address and port. */

    if (bind(listen_sock, (struct sockaddr*)&bind_addr, sizeof(bind_addr)) == -1) {
        fprintf(stderr, "bind port %d: ", port);
        perror(NULL);
        close(listen_sock);
        return -1;
    }

    /* Mark socket passive and set backlog of pending connections. */

    if (listen(listen_sock, 3) == -1) {
        perror("listen");
        close(listen_sock);
        return -1;
    }

    printf("Waiting for incoming connections on port %d...\n", port);

    /* Block until a client connects; data_sock is dedicated to that one peer. */

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int data_sock = accept(listen_sock, (struct sockaddr*)&client_addr, &client_len);
    if (data_sock == -1) {
        perror("accept");
        close(listen_sock);
        return -1;
    }

    printf("Connection accepted from %s:%d\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    /* One client only — drop the listener socket like before. */

    close(listen_sock);
    return data_sock;

}

/* Public open: NULL address => server mode; non-NULL => client mode to that host. */

int socket_open(const char* address, int port) {
    if (address != NULL) {
        return run_client(address, port);
    }
    return run_server(port);
}

/* Send one full packet_t over TCP; loop handles partial sends until entire struct is written. */

int socket_send(int handle, const packet_t* packet) {
    const int total_size = (int)sizeof(packet_t);
    int sent_so_far = 0;
    const char* bytes = (const char*)packet;
    while (sent_so_far < total_size) {
        int n = send(handle, bytes + sent_so_far, total_size - sent_so_far, 0);
        if (n <= 0) {
            return -1;
        }
        sent_so_far += n;
    }
    return sent_so_far;
}

/* Receive exactly sizeof(packet_t) bytes; loop handles partial reads until buffer is full. */

int socket_receive(int handle, packet_t* packet) {
    const int total_size = (int)sizeof(packet_t);
    int got_so_far = 0;
    char* bytes = (char*)packet;
    while (got_so_far < total_size) {
        int n = recv(handle, bytes + got_so_far, total_size - got_so_far, 0);
        if (n <= 0) {
            return -1;
        }
        got_so_far += n;
    }
    return got_so_far;
}

/* Close the underlying socket file descriptor. */

void socket_close(int handle) {
    close(handle);
}

/* Vtable passed to send_file/receive_file: function pointers so transfer logic stays transport-agnostic. */

const transport_ops_t socket_transport = {
    .open = socket_open,
    .send = socket_send,
    .receive = socket_receive,
    .close = socket_close,
};


