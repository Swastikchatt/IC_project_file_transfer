#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "depacketizer.h"
#include "file_io.h"
#include "packetizer.h"
#include "transport.h"

extern const transport_ops_t socket_transport;

static void print_usage(const char* prog_name) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s send <filepath> <ip_address> <port>\n", prog_name);
    fprintf(stderr, "  %s receive <filepath> <port>\n", prog_name);
    fprintf(stderr, "  %s copy <source> <dest>\n", prog_name);
    fprintf(stderr, "  %s packetize <source> <dest_packet_file>\n", prog_name);
    fprintf(stderr, "  %s depacketize <packet_file> <dest_file>\n", prog_name);
}

static int usage_fail(const char* prog_name) {
    print_usage(prog_name);
    return 1;
}

/* atoi keeps the old behaviour (weird ports become 0 or nonsense — same as before). */
static int run_send(const char* prog, int argc, char** argv) {
    if (argc != 5) {
        return usage_fail(prog);
    }

    const char* file_path = argv[2];
    const char* host = argv[3];
    int port = atoi(argv[4]);

    printf("Mode: Send\n");
    printf("Filepath: %s\n", file_path);
    printf("IP Address: %s\n", host);
    printf("Port: %d\n", port);

    send_file(file_path, host, port, (transport_ops_t*)&socket_transport);
    return 0;
}

static int run_receive(const char* prog, int argc, char** argv) {
    if (argc != 4) {
        return usage_fail(prog);
    }

    const char* out_path = argv[2];
    int port = atoi(argv[3]);

    printf("Mode: Receive\n");
    printf("Filepath: %s\n", out_path);
    printf("Port: %d\n", port);

    receive_file(out_path, port, (transport_ops_t*)&socket_transport);
    return 0;
}

static int run_copy(const char* prog, int argc, char** argv) {
    if (argc != 4) {
        return usage_fail(prog);
    }
    local_file_copy(argv[2], argv[3]);
    return 0;
}

static int run_packetize(const char* prog, int argc, char** argv) {
    if (argc != 4) {
        return usage_fail(prog);
    }
    packetize_file(argv[2], argv[3]);
    return 0;
}

static int run_depacketize(const char* prog, int argc, char** argv) {
    if (argc != 4) {
        return usage_fail(prog);
    }
    depacketize_file(argv[2], argv[3]);
    return 0;
}

int main(int argc, char* argv[]) {
    const char* prog = argv[0];

    if (argc < 4) {
        return usage_fail(prog);
    }

    const char* mode = argv[1];

    if (strcmp(mode, "send") == 0) {
        return run_send(prog, argc, argv);
    }
    if (strcmp(mode, "receive") == 0) {
        return run_receive(prog, argc, argv);
    }
    if (strcmp(mode, "copy") == 0) {
        return run_copy(prog, argc, argv);
    }
    if (strcmp(mode, "packetize") == 0) {
        return run_packetize(prog, argc, argv);
    }
    if (strcmp(mode, "depacketize") == 0) {
        return run_depacketize(prog, argc, argv);
    }

    fprintf(stderr, "Unknown mode '%s' — expected send, receive, copy, packetize, or depacketize.\n", mode);
    return usage_fail(prog);
}
