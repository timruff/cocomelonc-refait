#include <iostream>
#include <windows.h>
#include <dbghelp.h>

#pragma comment (lib, "dbghelp.lib")

// callback function
BOOL CALLBACK PrintModules(
  PSTR ModuleName,
  ULONG ModuleBase,
  ULONG ModuleSize,
  PVOID UserContext) {
    // print the module name.
    printf("%s\n", ModuleName);
    return TRUE;
}

int main(int argc, char *argv[]) {
  // inject a DLL into remote process
  HANDLE ph = GetCurrentProcess();
  // enumerate modules
  printf("\nenumerate modules... \n");
  EnumerateLoadedModules(ph, (PENUMLOADED_MODULES_CALLBACK)PrintModules, NULL);
  return 0;
}