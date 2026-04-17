#ifndef CHECKSUM_H
#define CHECKSUM_H

#include "packet.h"

// Calculates a simple additive checksum for the packet's data payload.
uint32_t calculate_checksum(const packet_t* packet);

#endif 
