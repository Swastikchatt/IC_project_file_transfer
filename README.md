# Secure File Transfer Utility (C)

A modular, secure file transfer utility implemented in C for a systems programming project.

**This variant (`IC_project_file_transfer`) targets Linux and Unix-like systems only** — POSIX sockets, no Windows/Winsock support.

## Features

- **Modular Architecture**: Uses a transport abstraction layer (via function pointers) so transfer logic can use different transports (TCP, etc.).
- **Secure Transfer**: File data encrypted with AES-128 (CTR mode) using the `tiny-AES-c` library.
- **Data Integrity**: Additive checksum per packet to detect transit errors.
- **Packetization**: Splits files into fixed-size chunks (4KB).
- **Networking**: TCP via standard POSIX sockets (`<sys/socket.h>`, etc.).

## Project Structure

- `include/`: Headers for structures and interfaces.
- `src/`: File I/O, packetization, encryption, checksum, TCP transport.
- `lib/`: Third-party `tiny-AES-c`.
- `file_transfer`: Built executable (no `.exe` suffix on Unix).

## How to Build

On Linux or macOS (or WSL with GCC):

```bash
cd IC_project_file_transfer
make
```

Manual compile:

```bash
gcc -Iinclude -Wall -g source code/file_io.c source code/main.c source code/packetizer.c source code/transfer.c source code/trasport_socket.c source code/depacketizer.c source code/encryption.c source code/checksum.c source code/aes.c

## How to Use

### 1. Receive a File (Server Mode)

```bash
./file_transfer receive output_filename.txt <port>
```

Example:

```bash
./file_transfer receive received_file.txt 8080
```

### 2. Send a File (Client Mode)

```bash
./file_transfer send input_filename.txt <ip_address> <port>
```

Example:

```bash
./file_transfer send my_secret.txt 127.0.0.1 8080
```

### Other Commands (for testing)

- **Local Copy**: `./file_transfer copy <source> <dest>`
- **Packetize to File**: `./file_transfer packetize <source> <packet_bin>`
- **Depacketize from File**: `./file_transfer depacketize <packet_bin> <dest>`

# Member Contributions

M Megha Syam Harivardhan - Lead, CLI, integration, build (main.c, Makefile, README, demo, E2E tests).  


Swastik Chatterjee - Networking (transport.h, transport_socket.c, socket loops,       partial‑send write‑up).  


 Dharshan SS - Transfer logic (transfer.c, chunk flow, EOF).  


 Sreehari EH - Crypto + integrity (encryption.c/h, checksum.c/h, AES‑128‑CTR + IV tweak).  


NV Charan Teja - File I/O + packetize pipeline (file_io.c/h, packetizer.c, depacketizer.c, copy mode).

