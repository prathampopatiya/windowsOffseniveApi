#include<Windows.h>
#include<iostream>

using namespace std;

DWORD WINAPI MyThreadFunction(LPVOID lpParam){
    cout << "Thread running ... ID: " << GetCurrentThreadId()  << endl;
    Sleep(1000);
    return 0;
}

int main(){
    HANDLE hThread;
    DWORD threadId;

    hThread = CreateThread(
        NULL,
        0,
        MyThreadFunction,
        NULL,
        0,
        &threadId
    );
    if (hThread == NULL){
        cerr << "Thread creaation failed. Error " << GetLastError() << endl;
        return 1;
    }
    WaitForSingleObject(hThread,INFINITE);
    CloseHandle(hThread);

    return 0;
}