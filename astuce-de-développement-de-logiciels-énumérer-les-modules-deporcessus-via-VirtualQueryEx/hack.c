/*
 * hack.c - get the list of 
 * modules of the process via VirtualQueryEx. C++ implementation
 * @cocomelonc
 * https://cocomelonc.github.io/malware/2023/11/07/malware-tricks-37.html
*/
#include <windows.h>
#include <stdio.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <psapi.h>

#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "shlwapi.lib")

typedef NTSTATUS (NTAPI * fNtGetNextProcess)(
  _In_ HANDLE ph,
  _In_ ACCESS_MASK DesiredAccess,
  _In_ ULONG HandleAttributes,
  _In_ ULONG Flags,
  _Out_ PHANDLE Newph
);

int findMyProc(const char * procname) {
  int pid = 0;
  HANDLE current = NULL;
  char procName[MAX_PATH];

  // resolve function address
  fNtGetNextProcess myNtGetNextProcess = (fNtGetNextProcess) GetProcAddress(GetModuleHandle("ntdll.dll"), "NtGetNextProcess");

  // loop through all processes
  while (!myNtGetNextProcess(current, MAXIMUM_ALLOWED, 0, 0, &current)) {
    GetProcessImageFileNameA(current, procName, MAX_PATH);
    if (lstrcmpiA(procname, PathFindFileName((LPCSTR) procName)) == 0) {
      pid = GetProcessId(current);
      break;
    }
  }

  return pid;
}

// function to list modules loaded by a specified process
int listModulesOfProcess(int pid) {
  HANDLE ph;
  MEMORY_BASIC_INFORMATION mbi;
  char * base = NULL;

  ph = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
  if (ph == NULL)
    return -1;

  printf("modules found:\n");
  printf("name\t\t\t base address\n");
  printf("=================================================================================\n");

  while (VirtualQueryEx(ph, base, &mbi, sizeof(mbi)) == sizeof(MEMORY_BASIC_INFORMATION)) {
    char szModName[MAX_PATH];

    // only focus on the base address regions
    if ((mbi.AllocationBase == mbi.BaseAddress) && (mbi.AllocationBase != NULL)) {
      if (GetModuleFileNameEx(ph, (HMODULE) mbi.AllocationBase, (LPSTR) szModName, sizeof(szModName) / sizeof(TCHAR)))
        printf("%#25s\t\t%#10llx\n", szModName, (unsigned long long)mbi.AllocationBase);
      }
      // check the next region
      base += mbi.RegionSize;
    }
  
  CloseHandle(ph);
  return 0;
}

int main(int argc, char* argv[]) {
  int pid = 0; // process ID
  pid = findMyProc(argv[1]);
  printf("%s%d\n", pid > 0 ? "process found at pid = " : "process not found. pid = ", pid);
  if (pid != 0)
    listModulesOfProcess(pid);
  return 0;
}

