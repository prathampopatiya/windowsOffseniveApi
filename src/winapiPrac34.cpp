/*
In modern Windows environments, security tools such as EDRs (Endpoint Detection and Response) 
implement user-mode hooking to intercept and monitor process behavior. One of the most common methods is inline hooking, 
where security software modifies the beginning of critical functions (like system calls) to redirect execution to its own monitoring code.

This article demonstrates how to detect such inline hooks—particularly in the NTDLL.dll module, 
which contains the user-mode stubs for all Windows syscalls.

These syscall stubs have a recognizable prologue (a fixed sequence of machine instructions), and are expected to look like this (in x64):
    mov     r10, rcx
    mov     eax, <syscall_number>
    syscall
    ret

EDRs often patch this code by inserting a JMP instruction at the beginning of the function, which looks like:
    E9 XX XX XX XX
This is known as a trampoline—a redirect to EDR’s monitoring DLL (e.g., edrhook.dll). 
Detecting this change helps analysts determine if they’re in an instrumented environment.
*/
/*
* How Inline Hook Detection Works
* Load the NTDLL export directory.
* Iterate over functions starting with "Nt" or "Zw".
* Resolve the function address in memory.
* Read the first 4–5 bytes of the function.
* Compare with known syscall prologue (4C 8B D1 B8) or look for redirection patterns like:
* E9 (JMP)
* CC (INT3 breakpoint)
* Resolve and display the target of the JMP, and map it to the corresponding DLL.
*/

#include<Windows.h>
#include<winternl.h>
#include<iostream>
#include<string>
#include<Psapi.h>
#include<stdio.h>

#pragma comment(lib,"Psapi.lib")

int main(){
    HMODULE ntdllbase = LoadLibraryA("ntdll.dll");
    if(!ntdllbase){
        printf("%s LoadLibraryA ERROR=0x%08x",__FUNCTION__,GetLastError());
        return -1;
    }
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)ntdllbase;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(dos->e_lfanew+(BYTE*)ntdllbase);

    DWORD exportrva = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    PIMAGE_EXPORT_DIRECTORY exportDir = (PIMAGE_EXPORT_DIRECTORY)(exportrva+(BYTE*)ntdllbase);

    PDWORD functionRvas = (PDWORD)((BYTE*)ntdllbase+exportDir->AddressOfFunctions);
    PDWORD NameRvas = (PDWORD)((BYTE*)ntdllbase+exportDir->AddressOfNames);
    PWORD OrdinalRva = (PWORD)((byte*)ntdllbase+exportDir->AddressOfNameOrdinals);

    const BYTE syscallprolouge[] = {0x4c,0x8b,0xd1,0xb8};
    for(DWORD i=0;i<exportDir->NumberOfNames;i++){
        const char *functionName = (const char *)ntdllbase+NameRvas[i];

        if(strncmp(functionName,"Nt",2) != 0 && strncmp(functionName,"Zw",2) != 0) continue;

        WORD ordinal = OrdinalRva[i];
        DWORD funcRva = functionRvas[ordinal];
        BYTE *funcAddress = (byte*)ntdllbase+funcRva;

        if(memcmp(funcAddress,syscallprolouge,sizeof(syscallprolouge)) != 0){
            if(funcAddress[0] == 0xe9){
                DWORD reloffset = *(DWORD *)(funcAddress+1);
                BYTE *jmptarget = funcAddress+5+reloffset;
                
                char modulePath[MAX_PATH] = {};
                if(GetMappedFileNameA(GetCurrentProcess(),jmptarget,modulePath,MAX_PATH)){
                    cout << "[Hooked] " << functionName << "at " << (void *)funcAddress << "=> Jmp to" << (void*)jmptarget << "(module: " << modulePath << endl;
                }else{
                     std::cout << "[HOOKED] " << functionName << " at " << (void*)funcAddress
                              << " => JMP to " << (void*)jmpTarget
                              << " (unknown module)\n";
                }
            }else {
                std::cout << "[SUSPICIOUS] " << functionName << " at " << (void*)funcAddress
                          << " has unexpected prologue\n";
            }
        }
    }
    std::cout << "[+] Scan completed.\n";
    return 0;

}
