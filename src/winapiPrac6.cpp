#include<Windows.h>
#include<iostream>

using namespace std;

int main(){
    STARTUPINFO si = {sizeof(si)};
    PROCESS_INFORMATION pi;

    LPCSTR appName = "C:\\Windows\\System32\\notepad.exe";

    BOOL success = CreateProcessA(
        appName,
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );
    if(!success){
        cerr << "Failed to createProcess Error: " << GetLastError() << endl;
        return 1;
    }
    cout << "Process Created! PID" << pi.dwProcessId << endl;

    WaitForSingleObject(pi.hProcess,INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
