#include "file_io.h"

#include <stdio.h>

int read_file_chunk(FILE* fp, packet_t* packet) {
    if (fp == NULL || packet == NULL) {
        return -1;
    }

    size_t n = fread(packet->data, 1, MAX_DATA_SIZE, fp);
    packet->data_size = (uint32_t)n;
    return (int)n;
}

int write_file_chunk(FILE* fp, const packet_t* packet) {
    if (fp == NULL || packet == NULL) {
        return -1;
    }

    size_t n = fwrite(packet->data, 1, packet->data_size, fp);
    return (int)n;
}

void local_file_copy(const char* input_path, const char* output_path) {
    FILE* in = fopen(input_path, "rb");
    if (!in) {
        fprintf(stderr, "copy: can't read '%s': ", input_path);
        perror(NULL);
        return;
    }

    FILE* out = fopen(output_path, "wb");
    if (!out) {
        fprintf(stderr, "copy: can't write '%s': ", output_path);
        perror(NULL);
        fclose(in);
        return;
    }

    packet_t buf;
    int seq = 0;
    int chunk_len;

    printf("Starting local file copy from '%s' to '%s'...\n", input_path, output_path);

    while ((chunk_len = read_file_chunk(in, &buf)) > 0) {
        buf.sequence_number = seq++;
        buf.checksum = 0; /* copy mode doesn't crypto — checksum unused */

        printf("Read chunk %d: %d bytes\n", buf.sequence_number, chunk_len);

        if (write_file_chunk(out, &buf) != chunk_len) {
            fprintf(stderr, "copy: write failed on chunk %d\n", buf.sequence_number);
            break;
        }
    }

    if (ferror(in)) {
        perror("copy: read error");
    }

    printf("File copy complete.\n");

    fclose(in);
    fclose(out);
}
