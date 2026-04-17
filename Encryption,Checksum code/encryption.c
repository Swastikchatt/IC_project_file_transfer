#include "encryption.h"

#include "aes.h"
#include <string.h>

// XOR low bits of seq into the IV so two identical chunks don't encrypt the same. 
//iv --> initlising vector 
static void modif_init_v_with_sequence(uint8_t init_vect_out[16], const uint8_t init_vect_in[16], uint32_t seq) {
    memcpy(init_vect_out, init_vect_in, 16);
    init_vect_out[15] ^= (uint8_t)(seq & 0xFF);
    init_vect_out[14] ^= (uint8_t)((seq >> 8) & 0xFF);
    init_vect_out[13] ^= (uint8_t)((seq >> 16) & 0xFF);
    init_vect_out[12] ^= (uint8_t)((seq >> 24) & 0xFF);
}

void encrypt_packet(packet_t* packet, const uint8_t* key, const uint8_t* init_vect) {
    if (packet == NULL || key == NULL || init_vect == NULL) {
        return;
    }

    uint8_t init_v_this_packet[16];
    modif_init_v_with_sequence(init_v_this_packet, init_vect, packet->id);

    struct AES_ctxt ctx;
    AES_initial_ctxt_init_v(&ctx, key, init_v_this_packet);
    AES_CTR_xcrypt_buffer(&ctx, packet->data, packet->data_size);
}   

void decrypt_packet(packet_t* packet, const uint8_t* key, const uint8_t* iv) {
    encrypt_packet(packet, key, iv);
}
