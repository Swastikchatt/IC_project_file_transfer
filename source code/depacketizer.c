#include "depacketizer.h"

#include "encryption.h"
#include "file_io.h"
#include "packet.h"
#include <stdio.h>

static uint8_t s_key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
static uint8_t s_iv[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

void depacketize_file(const char* input_path, const char* output_path) {
    FILE* in = fopen(input_path, "rb");
    if (!in) {
        fprintf(stderr, "depacketize: open '%s': ", input_path);
        perror(NULL);
        return;
    }

    FILE* out = fopen(output_path, "wb");
    if (!out) {
        fprintf(stderr, "depacketize: create '%s': ", output_path);
        perror(NULL);
        fclose(in);
        return;
    }

    packet_t pkt;

    printf("Starting SECURE depacketization from '%s' to '%s'...\n", input_path, output_path);

    while (fread(&pkt, sizeof(packet_t), 1, in) == 1) {
        printf("Depacketizing chunk %d: %d bytes (Decrypting)\n", pkt.sequence_number, pkt.data_size);

        decrypt_packet(&pkt, s_key, s_iv);

        int w = write_file_chunk(out, &pkt);
        if (w != (int)pkt.data_size) {
            fprintf(stderr, "depacketize: write failed on seq %d\n", pkt.sequence_number);
            break;
        }
    }

    if (ferror(in)) {
        perror("depacketize: read error");
    }

    printf("Secure depacketization complete.\n");

    fclose(in);
    fclose(out);
}
