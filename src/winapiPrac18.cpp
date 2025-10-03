#include<windows.h>
#include<winternl.h>
#include<iostream>

using namespace std;

typedef struct _MY_PROCESS_BASIC_INFORMATION{
    PVOID Reserved1;
    PPEB PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
} MY_PROCESS_BASIC_INFORMATION;

typedef NTSTATUS(NTAPI* NtQueryInformationProcess_t)(
    HANDLE,
    PROCESSINFOCLASS,
    PVOID,
    ULONG,
    PULONG
);
int main(){
    DOWRD pid = GetCurrentProcessId();
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,false,pid);

    if(!hProcess){
        cerr << "Failed to open the process.Error: " << GetLastError() << endl;
        return 1;
    }
    HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
    NtQueryInformationProcess_t NtQueryInformationProcess = (NtQueryInformationProcess_t)GetProcAddress(hNtdll,"NtQueryInformationProcess");

    if(!NtQueryInformationProcess){
        cerr << "Could not resolve the NtQueryInformationProcess " << endl;
        Closehandle(hProcess);
        return 1;
    }
    MY_PROCESS_BASIC_INFORMATION pbi = {};
    ULONG returnLength = 0;
    NTSTATUS status = NtQueryInformationProcess(
        hProcess,
        ProcessBasicInformation,
        &pbi,
        sizeof(pbi),
        &returnLength
    );
    if(status == 0){
        cout << "PEB address: " << pbi.PebBaseAddress << endl;
        cout << "Parent Id: " << pbi.InheritedFromUniqueProcessId << endl;
    }else{
        cerr << "NtQueryInformationProcess failed. NTSTATUS: 0x" << hex << status << endl;
    }
    CloseHandle(hProcess);
    return 0;
}