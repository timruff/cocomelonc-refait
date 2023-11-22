/*
hack.cpp
anti-debugging via NtGlobalFLag
author: @cocomelonc
https://cocomelonc.github.io/malware/2022/09/15/malware-av-evasion-10.html
*/
#include <winternl.h>
#include <windows.h>
#include <stdio.h>

#define FLG_HEAP_ENABLE_TAIL_CHECK   0x10
#define FLG_HEAP_ENABLE_FREE_CHECK   0x20
#define FLG_HEAP_VALIDATE_PARAMETERS 0x40
#define NT_GLOBAL_FLAG_DEBUGGED (FLG_HEAP_ENABLE_TAIL_CHECK | FLG_HEAP_ENABLE_FREE_CHECK | FLG_HEAP_VALIDATE_PARAMETERS)

#pragma comment (lib, "user32.lib")

DWORD checkNtGlobalFlag() {
  PPEB ppeb = (PPEB)__readgsqword(0x60);
  DWORD myNtGlobalFlag = *(PDWORD)((PBYTE)ppeb + 0xBC);
  MessageBox(NULL, myNtGlobalFlag & NT_GLOBAL_FLAG_DEBUGGED ? "Bow-wow!" : "Meow-meow!", "=^..^=", MB_OK);
  return 0;
}

int main(int argc, char* argv[]) {
  DWORD check = checkNtGlobalFlag();
  return 0;
}

