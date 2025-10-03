#include<windows.h>
#include<iostream>

using namespace std;
int main(){
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};

    si.cb = sizeof(si);

    LPCWSTR appName = "";

    BOOL success = CreateProcessW(
        appName,
        NULL,
        NULL,
        NULL,
        false,
        0,
        NULL,
        NULL,
        &si,
        &pi,
    );
    if(success){
        wcout << L"Process Created with PID: " << pi.dwProcessId << endl;
        WaitForSingleObject(pi.hProcess,-1);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else{
        wcerr << L"Failed to create process. Error: " << GetLastError() << endl;
    }
    return 0;
}