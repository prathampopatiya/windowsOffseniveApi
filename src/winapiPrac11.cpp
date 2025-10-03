#include<Windows.h>
#include<iostream>

using namespace std;

int main(){
    DWORD pid;
    cout << "Enter target pid: ";
    cin >> pid;

    HANDLE hProcess = OpenProcess(
        PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
        FALSE,
        pid
    );
    if(!hProcess){
        cerr << "Failed to open process. Error" << GetLastError() << endl;
        return 1;
    }
    LPCVOID address = (LPCVOID)0x7ffdf000;
    char buffer[32] = {0};
    
    SIZE_T bytesRead;
    if(ReadProcessMemory(hProcess,address,buffer,sizeof(buffer),&bytesRead)){
        cout << "Data read (" << bytesRead << " bytes)" << endl;
    }else{
        cerr << "ReadProcessMemory failed. Error " << GetLastError() << endl; 
    }
    CloseHandle(hProcess);
    return 0;
}