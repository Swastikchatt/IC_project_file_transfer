#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

// Define a maximum data payload size for our packets
#define MAX_DATA_SIZE 4096

// Define the packet structure
typedef struct {
    uint32_t id; // To order the packets
    uint32_t data_size;       
    uint32_t checksum;// To verify data integrity
    unsigned char data[MAX_DATA_SIZE]; // The file data chunk
} packet_t;

#endif 
