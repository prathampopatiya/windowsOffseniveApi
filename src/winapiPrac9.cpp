#include<Windows.h>
#include<iostream>
using namespace std;

int main(){

    DWORD pid;
    cout << "Enter the PID of the proceess to open: ";
    cin >> pid;

    HANDLE hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE,
        pid
    );
    if(hProcess == NULL){
        cerr << "Faailed to open the process. Error" << GetLastError() << endl;
        return 1;
    }
    cout << "Process opened successfully (handle: " << hProcess << "). " << endl;

    CloseHandle(hProcess);
    return 0;
}