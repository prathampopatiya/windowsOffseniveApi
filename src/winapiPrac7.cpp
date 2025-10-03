#include<Windows.h>
#include<iostream>

using namespace std;

int main(){
    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    cout << "Current Process ID: " <<  pid <<endl;
    cout << "Current Thread ID: " <<  tid <<endl;

    return 0;
}

