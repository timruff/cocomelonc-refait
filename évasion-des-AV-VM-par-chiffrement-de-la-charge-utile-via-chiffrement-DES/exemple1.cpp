#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>

const BYTE payload[] = "\x90\x90\x90\x90\x90\x90\x90\x90"; // shellcode
const DWORD payload_size = sizeof(payload);
const BYTE key[] = "0123457"; // 8-byte DES key

int main(void) {
  HCRYPTPROV hProv = 0;
  HCRYPTKEY hKey = 0;
  BYTE encrypted[payload_size];
  DWORD encrypted_size = 0;
  CryptAcquireContext(&hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
  CryptCreateHash(hProv, CALG_MD5, 0, 0, &hKey);
  CryptHashData(hKey, key, sizeof(key), 0);
  CryptDeriveKey(hProv, CALG_DES, hKey, 0, &hKey);
  CryptEncrypt(hKey, 0, TRUE, 0, encrypted, &encrypted_size, payload_size);
  CryptDestroyKey(hKey);
  CryptReleaseContext(hProv, 0);

  printf("encrypted payload:\n");
  for (DWORD i = 0; i < encrypted_size; i++) {
      printf("\\x%02x", encrypted[i]);
  }
  printf("\n");

  return 0;
}
