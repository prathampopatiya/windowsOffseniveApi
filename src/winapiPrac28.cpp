#include<windows.h>
#include<iostream>
#include<winternl.h>

/*
These two NT Native APIs are used to map or unmap a section (memory-mapped file, shared memory, or image) 
into the virtual address space of a process.

They are commonly used in:

    DLL Hollowing / Process Hollowing
    NTDLL Unhooking (Remapping Clean Copy)
    Reflective/Manual DLL Mapping
    Direct System Call Stagers (e.g., Hell's Gate, SysWhispers)
    Malware loading techniques that bypass LoadLibrary and Windows loader

    NTSYSCALLAPI NTSTATUS NtMapViewOfSection(
    HANDLE          SectionHandle,
    HANDLE          ProcessHandle,
    PVOID           *BaseAddress,
    ULONG_PTR       ZeroBits,
    SIZE_T          CommitSize,
    PLARGE_INTEGER  SectionOffset,
    PSIZE_T         ViewSize,
    DWORD           InheritDisposition,
    ULONG           AllocationType,
    ULONG           Win32Protect
    );

    NTSYSCALLAPI NTSTATUS NtUnmapViewOfSection(
    HANDLE ProcessHandle,
    PVOID  BaseAddress
    );
*/

typedef NTSTATUS(NTAPI *pNtMapViewOfSection)(
    HANDLE,HANDLE,PVOID*,ULONG_PTR,SIZE_T,PLARGE_INTEGER,
    PSIZE_T,DWORD,ULONG,ULONG
);
typedef NTSTATUS(NTAPI *pNtUnMapviewOfSection)(HANDLE,PVOID);

int main(){
    HANDLE hFile = CreateFileW(
        "c:\\windows\\system32\\ntdll.dll",
        GENEREIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if(hFile == INVALID_HANDLE_VALUE){
        printf("[-] %s CreateFileW ERROR=0x%08x",__FUNCTION__,GetLastError());
        return -1;
    }
    HANDLE hSection = NULL;
    NTSTATUS status = NtCreateSection(
        &hSection,SECTION_MAP_READ,NULL,
        0,PAGE_READONLY,SEC_IMAGE,hFile
    );
    if(!status){
        printf("[-] %s NtCreateSection ERROR=0x%08x",GetLastError());
        CloseHandle(hSection);
        return -1;
    }
    size_t view_size = 0;
    PVOID baseAddr = NULL;

    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    pNtMapViewOfSection NtMap = (pNtMapViewOfSection)GetProcAddress(
        GetModuleHandleW(L"ntdll.dll"),"NtMapViewOfSection"
    );
    status = NtMap(hSection,hNtdll,&baseAddr,0,0,NULL,&view_size,ViewUnmap,0,PAGE_READONLY);
    if(!status){
        printf("[-] %s NtMap ERROR=0x%08x",GetLastError());
        CloseHandle(hSection);
        CloseHandle(hModule);
    }else{
        printf("Clean ntdll mapped at base_addr %p",basAddr);
        pNtUnmapViewOfSection NtUnmap = (pNtUnmapViewOfSection)GetProcAddress(
            GetModuleHandleW(L"ntdll.dll"),"NtUnmapViewOfSection"
        );
    }
    return 0;
}

