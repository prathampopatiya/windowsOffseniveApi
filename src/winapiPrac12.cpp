#include<Windows.h>
#include<iostream>
using namespace std;

int main(){
    DWORD pid;
    cout << "Enter the pid of the program: ";
    cin >> pid;

    HANDLE hProcess = OpenProcess(
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
        FALSE,
        pid
    );
    if(!hProcess){
        cerr << "Failed to open the process. Error " << GetLastError() << endl;
        return 1;
    }
    LPVOID remoteMem = VirtualAllocEx(
        hProcess,
        NULL,
        1024,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    if(!remoteMem){
        cerr << "Failed to allocate memory to the process" << endl;
        CloseHandle(hProcess);
        return 1;
    }
    cout << "memory allocated at remote address: " << remoteMem << endl;
    return 0;
}