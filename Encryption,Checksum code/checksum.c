#include "checksum.h"

uint32_t calculate_checksum(const packet_t* packet) {
    if (packet == NULL) {
        return 0;
    }
    else{
        uint32_t sum = 0;
        for (uint32_t i = 0; i < packet->data_size; i++) {
            sum += packet->data[i];
        }
        return sum;
    }
}
