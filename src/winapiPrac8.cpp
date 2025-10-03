#include<Windows.h>
#include<iostream>

using namespace std;

int main(){
    HANDLE hfile = CreateFileA(
        "example.txt",
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if(hfile == INVALID_HANDLE_VALUE){
        cerr << "Failed opening the file" << GetLastError() << endl;
        return 1;
    }
    char buffer[128] = {0};
    DWORD bytesRead;

    BOOL success = ReadFile(
        hfile,
        buffer,
        sizeof(buffer),
        &bytesRead,
        NULL
    );

    if(!success){
        cerr << "Failed to read the data" << GetLastError()  << endl;
    }else{
        cout << "Read: " << bytesRead << endl;
    }
    CloseHandle(hfile);
    return 0;
}