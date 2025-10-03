#include<Windows.h>
#include<iostream>

using namespace std;

int main(){
    DWORD pid;
    cout << "Enter the pid: ";
    cin >> pid;

    HANDLE hProcess = OpenProcess(
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE,
        pid
    );
    if (!hProcess) {
        std::cerr << "OpenProcess failed. Error: " << GetLastError() << std::endl;
        return 1;
    }
    LPVOID remoteAddr = VirtualAllocEx(
        hProcess,
        NULL,
        256,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    const char *message = "Hello from CXX to your memory";

    SIZE_T bytesWritten;
    if(!WriteProcessMemory(hProcess,remoteAddr,message,strlen(message)+1,&bytesWritten)){
        cout << "Successfully wrote " << bytesWritten << " bytes to remote process" << endl;
    }else{
        cerr << "WriteProcessMemory Failed. Error" << GetLastError() << endl;
    }
    CloseHandle(hProcess);
    return 0;
}