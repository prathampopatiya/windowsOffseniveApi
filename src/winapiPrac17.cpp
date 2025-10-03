#include<windows.h>
#include<iostream>

using namespace std;
int main(){
    int targetValue = 1337;
    cout << "[+] orginal value: " << targetValue << endl;

    DWORD pid = GetCurrentProcessId();
    cout << "[+] Current PID: " << pid << endl;

    HANDLE hProc = OpenProcess(
        PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERAATION,
        FALSE,
        pid
    );
    if(!hProc){
        cerr << "[-] Failed to open the process. Error: " << GetLastError() << endl;
        return 1;
    }

    int readBuffer = 0;
    SIZE_T bytesRead = 0;

    if(ReadProcessMemory(hProc,&targetValue,&readBuffer,sizeof(readBuffer),&bytesRead)){
        cout << "[+] ReadProcessMemory: " << readBuffer << " (" << bytesread << " bytes )" << endl;
    }else{
        cout << "[-] Failed to read process memory.Error: " << GetLastError() << endl;
    }
    int newValue = 9000;
    SIZE_T bytesWritten = 0;

    if(WriteProcessMemory(hProc,&targetValue,&newValue,sizeof(newValue),&bytesWritten)){
        cout << "Successfully wrote to process memory" << bytesWritten << " bytes " << endl;
    }else{
        cerr << "[-] failed to write to Memory.Error: " << GetLastError() << endl;
    }
    CloseHandle(hProc);
    return 0;
}

