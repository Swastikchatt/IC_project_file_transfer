# Secure File Transfer Utility (C)

A modular, secure file transfer utility implemented in C for a systems programming project.

## Features

- **Modular Architecture**: Uses a transport abstraction layer (via function pointers) allowing easy swapping between different transport methods (e.g., Sockets, Files, or even simulated QR codes).
- **Secure Transfer**: All file data is encrypted using AES-128 (CTR mode) using the `tiny-AES-c` library.
- **Data Integrity**: Includes a simple additive checksum for each packet to detect transit errors.
- **Packetization**: Handles files of any size by splitting them into fixed-size chunks (4KB).
- **Cross-Platform Networking**: Support for Windows (Winsock2) and Linux (Standard Sockets).

## Project Structure

- `include/`: Header files defining the core structures and interfaces.
- `src/`: Implementation files for File I/O, Packetization, Encryption, Checksum, and Networking.
- `lib/`: Third-party libraries (`tiny-AES-c`).
- `file_transfer.exe`: The compiled executable.

## How to Build

Using GCC on Windows:
```bash
gcc -Iinclude -Wall -g src/main.c src/file_io.c src/transport_socket.c src/packetizer.c src/depacketizer.c src/encryption.c src/transfer.c src/checksum.c lib/aes.c -o file_transfer.exe -lws2_32
```

## How to Use

### 1. Receive a File (Server Mode)
Run the utility in `receive` mode first on the receiving machine (or in a separate terminal):
```bash
.\file_transfer.exe receive output_filename.txt <port>
```
Example:
```bash
.\file_transfer.exe receive received_file.txt 8080
```

### 2. Send a File (Client Mode)
Run the utility in `send` mode on the sending machine:
```bash
.\file_transfer.exe send input_filename.txt <ip_address> <port>
```
Example:
```bash
.\file_transfer.exe send my_secret.txt 127.0.0.1 8080
```

### Other Commands (for testing)
- **Local Copy**: `.\file_transfer.exe copy <source> <dest>`
- **Packetize to File**: `.\file_transfer.exe packetize <source> <packet_bin>`
- **Depacketize from File**: `.\file_transfer.exe depacketize <packet_bin> <dest>`

## Systems Programming Concepts Covered

1. **File I/O**: Efficiently reading and writing binary data using `fread` and `fwrite`.
2. **Memory Management**: Working with structs and buffers to represent network packets.
3. **Data Serialization**: Representing complex structures in a format suitable for transmission.
4. **Network Programming**: Socket creation, binding, listening, connecting, and data transmission.
5. **Cryptography**: Integrating a third-party encryption library and managing initialization vectors (IVs).
6. **Error Detection**: Implementing checksums to ensure data has not been corrupted.
7. **Abstraction**: Using function pointers to create a "transport interface," a common pattern in system-level code (like the Linux kernel).
