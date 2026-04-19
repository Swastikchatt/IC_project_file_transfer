#include "packetizer.h"

#include "encryption.h"
#include "file_io.h"
#include "packet.h"
#include <stdio.h>

/* Same demo key as transfer.c — keep in sync if you change one. */
static uint8_t s_key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
static uint8_t s_iv[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

void packetize_file(const char* input_path, const char* output_path) {
    FILE* in = fopen(input_path, "rb");
    if (!in) {
        fprintf(stderr, "packetize: open '%s': ", input_path);
        perror(NULL);
        return;
    }

    FILE* out = fopen(output_path, "wb");
    if (!out) {
        fprintf(stderr, "packetize: create '%s': ", output_path);
        perror(NULL);
        fclose(in);
        return;
    }

    packet_t pkt;
    int seq = 0;
    int nbytes;

    printf("Starting SECURE packetization from '%s' to '%s'...\n", input_path, output_path);

    while ((nbytes = read_file_chunk(in, &pkt)) > 0) {
        pkt.sequence_number = seq++;
        pkt.checksum = 0; /* unchanged from original — wire-transfer mode is the one that checksums */

        encrypt_packet(&pkt, s_key, s_iv);

        printf("Packetizing chunk %d: %d bytes (Encrypted)\n", pkt.sequence_number, nbytes);

        if (fwrite(&pkt, sizeof(packet_t), 1, out) != 1) {
            fprintf(stderr, "packetize: fwrite failed on seq %d\n", pkt.sequence_number);
            break;
        }
    }

    if (ferror(in)) {
        perror("packetize: read error");
    }

    printf("Secure packetization complete.\n");

    fclose(in);
    fclose(out);
}
