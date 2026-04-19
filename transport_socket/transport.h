#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "packet.h"
#include <stddef.h>

/*
 * Thin vtable-style transport so transfer.c doesn't care if we're on TCP,
 * a pipe, or something silly for class demos.
 */
typedef struct transport_ops {
    int (*open)(const char* address, int port);
    int (*send)(int handle, const packet_t* packet);
    int (*receive)(int handle, packet_t* packet);
    void (*close)(int handle);
} transport_ops_t;

void send_file(const char* filepath, const char* address, int port, transport_ops_t* transport);
void receive_file(const char* filepath, int port, transport_ops_t* transport);

#endif
