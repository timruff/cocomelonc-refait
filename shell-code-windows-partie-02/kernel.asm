section .data
section .bss
section .text
    global _start

_start:
    ; Get the PEB address
    mov eax, [fs:0x30]

    ; Get the Ldr pointer
    mov eax, [eax + 0x0C]

    ; Get the InMemoryOrderModuleList pointer
    mov eax, [eax + 0x14]

    ; Get the current program's module (this program)
    mov eax, [eax]

    ; Get the ntdll module
    mov eax, [eax]

    ; Get the DllBase of kernel32
    mov eax, [eax + 0x10]

    ; ebx = 0 (just for a breakpoint)
    xor ebx, ebx

    ; Exit the program
    mov eax, 1          ; syscall number for sys_exit
    int 0x80            ; call kernel
