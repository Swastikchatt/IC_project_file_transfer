#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "packet.h"
#include <stdint.h>

// Encrypts the data payload of a packet in-place.
void encrypt_packet(packet_t* packet, const uint8_t* key, const uint8_t* iv);

// Decrypts the data payload of a packet in-place.
void decrypt_packet(packet_t* packet, const uint8_t* key, const uint8_t* iv);

#endif 
